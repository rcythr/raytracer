#include "kernel.hpp"

#include "util/sample_hemisphere.hpp"
#include "util/random_utils.hpp"
#include "util/thread_pool.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <chrono>

using namespace raytracer;

glm::vec3 Kernel::lookup_color(std::string name) {
    auto find_itr = colors.find(name);
    if (find_itr != colors.end()) {
        return find_itr->second;
    }
    throw std::runtime_error(std::string("Could not find color: '") + name + "'");
}

MaterialPtr Kernel::lookup_material(std::string name) {
    auto find_itr = materials.find(name);
    if (find_itr != materials.end()) {
        return find_itr->second;
    }
    throw std::runtime_error(std::string("Could not find material: '") + name + "'");
}

void Kernel::open(std::string& tag, ParamMap& params) {
    auto find_itr = handlers.find(tag);
    if (find_itr != handlers.end()) {
        find_itr->second(params);
    }
}

struct SpawnRaysContext {
    Kernel* kernel;
    CameraPtr camera;
    size_t num_bounces;
};

void spawn_rays_cb(void* ct, std::vector<RayContext>* row) {
    SpawnRaysContext* ctx = (SpawnRaysContext*)ct;

    size_t len = row->size();
    for(size_t i=0; i < len; ++i)
    {
        auto& ray_ctx = (*row)[i];
        ctx->camera->view_plane->add_pixel(ray_ctx.row, ray_ctx.col, ctx->kernel->get_color_rec(ray_ctx.ray, 1, ctx->num_bounces));
    }

    delete row;
}

struct LPHitContext {
    std::vector<Photon>& global_raw_photons;
    std::vector<Photon>& caustic_raw_photons;

    Kernel* kernel;
    size_t num_diffuse_hits;
    size_t num_specular_hits;
    glm::vec3 power, color;
    Ray* r;

    size_t TTL;
    bool destroyed;
};

void light_pass_hit(void* ct, HitResult& hit) {
    if(hit.shape->is_light)
        return;

    LPHitContext* ctx = (LPHitContext*)ct;
    if(ctx->TTL == 0)
        return;

    ctx->TTL -= 1;

    auto& mat = hit.shape->material;

    float Pd = mat->kd;
    float Ps = mat->ks;
    float Pt = mat->kt;

    float r1 = uniform_random();
    if(r1 < Pd) {
        // Diffuse Reflection
        ctx->num_diffuse_hits += 1;
            
        Photon p{hit.intersection_point, hit.incoming_ray.direction, ctx->power, mat->get_raw_color(hit) * ctx->color};

        if(ctx->num_diffuse_hits > 1) {
            ctx->global_raw_photons.push_back(p);
        }

        if(ctx->num_diffuse_hits == 1 && ctx->num_specular_hits > 0) {
            ctx->caustic_raw_photons.push_back(p);
        }

        ctx->r->origin = hit.intersection_point - hit.incoming_ray.direction * 0.05f;
        ctx->r->direction = glm::normalize(sampleHemisphereUniform(hit.intersection_normal, uniform_random(), uniform_random()));
        ctx->r->update();

        ctx->destroyed = false;
    } else if(r1 < (Pd + Ps)) {
        ctx->num_specular_hits += 1;

        // Specular Reflection
        // Update the ray and continue bouncing.
        ctx->r->origin = hit.intersection_point - hit.incoming_ray.direction * 0.05f;
        ctx->r->direction = glm::normalize(glm::reflect(hit.incoming_ray.direction, hit.intersection_normal));
        ctx->r->update();
        ctx->destroyed = false;
    } else if(r1 < (Pd + Ps + Pt)) {
        ctx->num_specular_hits += 1;

        // Specular Transmission
        // Update the ray and continue bouncing.

        float dir = glm::dot(hit.intersection_normal, hit.incoming_ray.direction);

        auto normal = ((dir <= 0.0f) ? -1.0f : 1.0f) * hit.intersection_normal;

        auto refract1 = glm::refract( hit.incoming_ray.direction, normal, ctx->kernel->world_ki / mat->ki );
        if(refract1 != glm::vec3(0.0f))
        {
            refract1 = glm::normalize(refract1);

            auto refract2 = (mat->is_hollow) ? glm::refract( refract1, -normal, mat->ki / ctx->kernel->world_ki ) : refract1;
            if(refract2 != glm::vec3(0.0f))
            {
                refract2 = glm::normalize(refract2);
                ctx->r->origin = hit.intersection_point + refract2 * 0.05f;
                ctx->r->direction = refract2;
                ctx->r->update();
            }
            else
            {
                ctx->r->origin = hit.intersection_point - refract1 * 0.05f;
                ctx->r->direction = glm::normalize(glm::reflect(-refract1, -normal));
                ctx->r->update();
            }
        }
        else
        {
            ctx->r->origin = hit.intersection_point - hit.incoming_ray.direction * 0.05f;
            ctx->r->direction = glm::normalize(glm::reflect(-hit.incoming_ray.direction, normal));
            ctx->r->update();
        }

        ctx->destroyed = false;
    } else {
        // Absorbed
        Photon p{hit.intersection_point,
                 hit.incoming_ray.direction,
                 ctx->power,
                 mat->get_raw_color(hit) * ctx->color};

        ctx->global_raw_photons.push_back(p);

        // Mark this photon as destroyed
        ctx->destroyed = true;
    }
}

void Kernel::lightPass() {
    std::vector<Photon> global_raw_photons;
    std::vector<Photon> caustic_raw_photons;

    std::vector<Ray> photon_rays;

    LPHitContext ctx{global_raw_photons, caustic_raw_photons, this, 0, 0, glm::vec3(0.0f), glm::vec3(1.0f), nullptr, false};
    
    Ray r;
    size_t rays_per_light = num_photons / lights.size();
    size_t num_calcs = 0;
    size_t max_calcs = num_photons * 10;
    while(num_calcs < max_calcs && global_raw_photons.size() < num_photons) {
        for(size_t i=0; i < lights.size(); ++i) {
            if(lights[i]->gen_photon(r)) {

                ctx.power = glm::vec3(lights[i]->intensity / rays_per_light);
                ctx.color = glm::vec3(1.0f);

                // Run every ray we fired into the scene to determine the hits
                ctx.TTL = 10;
                ctx.num_diffuse_hits = 0;
                ctx.num_specular_hits = 0;
                ctx.destroyed = false;
                ctx.r = &r;

                // We want to trace the photon around the scene until it is either lost or absorbed.
                while(!ctx.destroyed) {
                    // We mark it as destroyed, and will unmark it in the callback, if necessary.
                    ctx.destroyed = true;
                    spatial_index->find_closest_hit(r, light_pass_hit, &ctx, nullptr); 
                }
            }
        }
    }

    std::cout << global_raw_photons.size() << std::endl;

    if(global_raw_photons.size() != 0)
        global_photons.construct(global_raw_photons);
    
    if(caustic_raw_photons.size() != 0)
        caustic_photons.construct(caustic_raw_photons);
}

void Kernel::renderPass() {
    typedef std::chrono::high_resolution_clock Clock;

    size_t i = 1;
    for (CameraPtr& camera : cameras) {
        // Output the view of the photons from this camera.
        auto start = Clock::now();
        
        SpawnRaysContext ctx{this, camera, camera->get_num_bounces()};

        ThreadPool<std::vector<RayContext>*> tp(num_threads, spawn_rays_cb);

        // Give the camera a function to call on each ray it generates.
        auto rays = camera->spawn_rays();
        for(size_t i=0; i < rays.size(); ++i)
        {
            tp.enqueue(&ctx, rays[i]);
        }

        tp.stop();
        tp.join();

        // Signal to the view plane that we're finished writing. -  It divide by num_samples.
        camera->view_plane->finish(pow(camera->get_num_samples(), 2));
        
        // Apply all specified tone operators
        for(std::shared_ptr<ToneOperator> to : tone_operators) {
            to->apply(camera->view_plane);
        }

        // Save the contents of the view plane to the output file.
        camera->view_plane->save();

        auto end = Clock::now();

        std::cout << "Camera " << i << " Render Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(
                         end - start).count() << " miliseconds" << std::endl;

        ++i;
    }
}

struct ColorRecContext {
    Kernel* kernel;
    const Ray& ray;
    size_t num_bounces;
    size_t max_bounces;
    bool& hit_found;
    glm::vec3& result;
};

void color_rec_callback(void* ct, HitResult& hit) {
    ColorRecContext* ctx = (ColorRecContext*)ct;
    // Get Direct Illumination
    ctx->result = hit.shape->material->get_color(ctx->kernel, hit);

    float ks = hit.shape->material->ks;
    float kt = hit.shape->material->kt;
    float ki = hit.shape->material->ki;
    bool is_hollow = hit.shape->material->is_hollow;

    if(ctx->num_bounces < ctx->max_bounces)
    {
        // Get Reflected Illumination
        if(ks > 0.0)
        {
            Ray refl{hit.intersection_point - hit.incoming_ray.direction * 0.05f, glm::reflect(hit.incoming_ray.direction, hit.intersection_normal)};
            refl.update();
            ctx->result += ks * ctx->kernel->get_color_rec(refl, ctx->num_bounces + 1, ctx->max_bounces);
        }

        // Get Transmitted Illumination
        if(kt > 0.0)
        {
            float dir = glm::dot(hit.intersection_normal, hit.incoming_ray.direction);

            auto normal = ((dir <= 0.0f) ? -1.0f : 1.0f) * hit.intersection_normal;

            auto refract1 = glm::refract( hit.incoming_ray.direction, normal, ctx->kernel->world_ki / ki );
            if(refract1 != glm::vec3(0.0f))
            {
                refract1 = glm::normalize(refract1);

                auto refract2 = (is_hollow) ? glm::refract( refract1, -normal, ki / ctx->kernel->world_ki ) : refract1;
                if(refract2 != glm::vec3(0.0f))
                {
                    refract2 = glm::normalize(refract2);

                    Ray trans{ hit.intersection_point + refract2 * 0.05f, refract2 };
                    trans.update();
                    ctx->result += kt * ctx->kernel->get_color_rec(trans, ctx->num_bounces + 1, ctx->max_bounces);
                }
                else
                {
                    Ray refl{hit.intersection_point - refract1 * 0.05f, glm::normalize(glm::reflect(-refract1, -normal))};
                    refl.update();
                    ctx->result += kt * ctx->kernel->get_color_rec(refl, ctx->num_bounces + 1, ctx->max_bounces);
                }
            }
            else
            {
                Ray refl{hit.intersection_point - hit.incoming_ray.direction * 0.05f, glm::normalize(glm::reflect(-hit.incoming_ray.direction, normal))};
                refl.update();
                ctx->result += kt * ctx->kernel->get_color_rec(refl, ctx->num_bounces + 1, ctx->max_bounces);
            }
        }
    }
    ctx->hit_found = true;
}

glm::vec3 Kernel::get_color_rec(const Ray& ray, size_t num_bounces, size_t max_bounces)
{
    bool hit_found = false;
    glm::vec3 result;

    ColorRecContext ctx{this, ray, num_bounces, max_bounces, hit_found, result};

    spatial_index->find_closest_hit(ray, color_rec_callback, &ctx);

    // Determine if we need the background color.
    if(!hit_found) {
        return background_color;
    }

    return result;
}
