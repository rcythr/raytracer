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

void Kernel::lightPass() {
    RawPhotonMap global_raw_photons;
    std::vector<Photon> caustic_raw_photons;

    std::vector<Ray> photon_rays;

    size_t num_diffuse_hits = 0;
    size_t num_specular_hits = 0;
    glm::vec3 power, color;

    size_t num_emit_photons = 0;
    size_t TTL = 0;
    bool destroyed = false;

    Ray r;
    size_t rays_per_light = num_photons / lights.size();

    size_t total_emit_photons = 0;
    size_t num_calcs = 0;
    size_t max_calcs = num_photons * 10;
    while(num_calcs < max_calcs && total_emit_photons < num_photons) {
        for(size_t i=0; i < lights.size(); ++i) {
            if(lights[i]->gen_photon(r)) {

                power = glm::vec3(lights[i]->intensity / rays_per_light);
                color = glm::vec3(1.0f);

                // Run every ray we fired into the scene to determine the hits
                num_emit_photons = 0;
                TTL = 10;
                num_diffuse_hits = 0;
                num_specular_hits = 0;
                destroyed = false;

                // We want to trace the photon around the scene until it is either lost or absorbed.
                while(!destroyed) {
                    // We mark it as destroyed, and will unmark it in the callback, if necessary.
                    destroyed = true;
                    spatial_index->find_closest_hit(r, [&](HitResult& hit){
                        if(hit.shape->is_light)
                            return;

                        if(TTL == 0)
                            return;
                        TTL -= 1;

                        auto& mat = hit.shape->material;

                        float Pd = mat->kd;
                        float Ps = mat->ks;
                        float Pt = mat->kt;

                        float r1 = uniform_random();
                        if(r1 < Pd) {
                            // Diffuse Reflection
                            num_diffuse_hits += 1;

                            Photon p{hit.intersection_point, hit.incoming_ray.direction, power, mat->get_raw_color(hit) * color};

                            if(num_diffuse_hits > 1) {
                                insert_photon(global_raw_photons, hit.shape, p);
                                num_emit_photons += 1;
                            }

                            if(num_diffuse_hits == 1 && num_specular_hits > 0) {
                                caustic_raw_photons.push_back(p);
                            }

                            r.origin = hit.intersection_point - hit.incoming_ray.direction * 0.05f;
                            r.direction = glm::normalize(sampleHemisphereUniform(hit.intersection_normal, uniform_random(), uniform_random()));
                            r.update();

                            destroyed = false;
                        } else if(r1 < (Pd + Ps)) {
                            num_specular_hits += 1;

                            // Specular Reflection
                            // Update the ray and continue bouncing.
                            r.origin = hit.intersection_point - hit.incoming_ray.direction * 0.05f;
                            r.direction = glm::normalize(glm::reflect(hit.incoming_ray.direction, hit.intersection_normal));
                            r.update();
                            destroyed = false;
                        } else if(r1 < (Pd + Ps + Pt)) {
                            num_specular_hits += 1;

                            // Specular Transmission
                            // Update the ray and continue bouncing.

                            float dir = glm::dot(hit.intersection_normal, hit.incoming_ray.direction);

                            auto normal = ((dir <= 0.0f) ? 1.0f : -1.0f) * hit.intersection_normal;

                            auto refract1 = glm::refract( hit.incoming_ray.direction, normal, world_ki / mat->ki );
                            if(refract1 != glm::vec3(0.0f))
                            {
                                refract1 = glm::normalize(refract1);

                                auto refract2 = (mat->is_hollow) ? glm::refract( refract1, normal, mat->ki / world_ki ) : refract1;
                                if(refract2 != glm::vec3(0.0f))
                                {
                                    refract2 = glm::normalize(refract2);
                                    r.origin = hit.intersection_point + refract2 * 0.05f;
                                    r.direction = refract2;
                                    r.update();
                                }
                                else
                                {
                                    r.origin = hit.intersection_point - refract1 * 0.05f;
                                    r.direction = glm::normalize(glm::reflect(-refract1, -normal));
                                    r.update();
                                }
                            }
                            else
                            {
                                r.origin = hit.intersection_point - hit.incoming_ray.direction * 0.05f;
                                r.direction = glm::normalize(glm::reflect(-hit.incoming_ray.direction, normal));
                                r.update();
                            }

                            destroyed = false;
                        } else {
                            // Absorbed
                            Photon p{hit.intersection_point,
                                     hit.incoming_ray.direction,
                                     power,
                                     mat->get_raw_color(hit) * color};

                            insert_photon(global_raw_photons, hit.shape, p);
                            num_emit_photons += 1;

                            // Mark this photon as destroyed
                            destroyed = true;
                        }
                    });
                }

                total_emit_photons += num_emit_photons;
            }
        }
    }

    std::cout << global_raw_photons.size() << std::endl;

    if(global_raw_photons.size() != 0) {
        for(auto& pair : global_raw_photons)
        {
            pair.first->global_photons.construct(pair.second);
        }
    }

    if(caustic_raw_photons.size() != 0)
        caustic_photons.construct(caustic_raw_photons);
}

void Kernel::renderPass() {
    typedef std::chrono::high_resolution_clock Clock;

    size_t i = 1;
    for (CameraPtr& camera : cameras) {
        // Output the view of the photons from this camera.
        auto start = Clock::now();

        ThreadPool<std::function<void()>> tp(num_threads);
        // Give the camera a function to call on each ray it generates.
        camera->spawn_rays([&](size_t row, size_t col, Ray& ray){
            tp.enqueue([=]() {

                if(col == 0)
                    std::cout << "Processing row " << row << "\n";

                camera->view_plane->add_pixel(row, col, get_color_rec(ray, 1, camera->get_num_bounces()));
            });
        });
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

glm::vec3 Kernel::get_color_rec(const Ray& ray, size_t num_bounces, size_t max_bounces)
{
    bool hit_found = false;
    glm::vec3 result;

    spatial_index->find_closest_hit(ray, [&](HitResult& hit){
        // Get Direct Illumination
        result = hit.shape->material->get_color(this, hit);

        float ks = hit.shape->material->ks;
        float kt = hit.shape->material->kt;
        float ki = hit.shape->material->ki;
        bool is_hollow = hit.shape->material->is_hollow;

        if(num_bounces < max_bounces)
        {
            // Get Reflected Illumination
            if(ks > 0.0)
            {
                Ray refl{hit.intersection_point - hit.incoming_ray.direction * 0.05f, glm::reflect(hit.incoming_ray.direction, hit.intersection_normal)};
                refl.update();
                result += ks * get_color_rec(refl, num_bounces + 1, max_bounces);
            }

            // Get Transmitted Illumination
            if(kt > 0.0)
            {
                float dir = glm::dot(hit.intersection_normal, hit.incoming_ray.direction);

                auto normal = ((dir <= 0.0f) ? 1.0f : -1.0f) * hit.intersection_normal;

                auto refract1 = glm::refract( hit.incoming_ray.direction, normal, world_ki / ki );
                if(refract1 != glm::vec3(0.0f))
                {
                    refract1 = glm::normalize(refract1);

                    auto refract2 = (is_hollow) ? glm::refract( refract1, normal, ki / world_ki ) : refract1;
                    if(refract2 != glm::vec3(0.0f))
                    {
                        refract2 = glm::normalize(refract2);

                        Ray trans{ hit.intersection_point + refract2 * 0.05f, refract2 };
                        trans.update();
                        result += kt * get_color_rec(trans, num_bounces + 1, max_bounces);
                    }
                    else
                    {
                        Ray refl{hit.intersection_point - refract1 * 0.05f, glm::normalize(glm::reflect(-refract1, -normal))};
                        refl.update();
                        result += kt * get_color_rec(refl, num_bounces + 1, max_bounces);
                    }
                }
                else
                {
                    Ray refl{hit.intersection_point - hit.incoming_ray.direction * 0.05f, glm::normalize(glm::reflect(-hit.incoming_ray.direction, normal))};
                    refl.update();
                    result += kt * get_color_rec(refl, num_bounces + 1, max_bounces);
                }
            }
        }
        hit_found = true;
    });

    // Determine if we need the background color.
    if(!hit_found) {
        return background_color;
    }

    return result;
}
