#include "kernel.hpp"

#include "util/split.hpp"
#include "util/xml_helpers.hpp"
#include "util/string_mult.hpp"
#include "util/vec3_helpers.hpp"
#include "util/obj_mesh.hpp"

#include "util/thread_pool.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

using namespace raytracer;

Kernel::Kernel() {
    verbose = true;

    handlers = {

        // General
        { "general", [this](ParamMap& params) {
            verbose = extractBool(params, "verbose");
            background_color = extractVec3(params, "background_color");
            num_threads = extractInt(params, "num_threads", std::max(std::thread::hardware_concurrency(), 1u));
            world_ki = extractFloat(params, "world_ki", 1.000293);
        } },

        // Spatial Indecies
        { "kdtree_index", [this](ParamMap& params) {
            spatial_index = std::make_shared<KDTreeSpatialIndex>();
        } },

        // Cameras
        { "pinhole", [this](ParamMap& params) {
            cameras.push_back(std::make_shared<PinholeCamera>(
                extractFloat(params, "pixel_size"),
                extractInt(params, "num_samples"),
                extractInt(params, "num_bounces"),
                extractVec3(params, "eye"),
                extractVec3(params, "up"),
                extractVec3(params, "look_at"),
                extractFloat(params, "view_distance")));
        } },

        // ViewPlanes
        { "ppm_viewplane", [this](ParamMap& params) {
            cameras.at(cameras.size() - 1)->view_plane =
                std::make_shared<PPMViewPlane>(
                    extractInt(params, "hres"),
                    extractInt(params, "vres"),
                    extractString(params, "filename"));
        } },

        // Lights
        { "ambient", [this](ParamMap& params) {
            lights.push_back(std::make_shared<AmbientLight>(
                extractFloat(params, "intensity"),
                lookup_color(extractString(params, "color"))));
        } },

        { "directional", [this](ParamMap& params) {
            lights.push_back(std::make_shared<DirectionalLight>(
                extractFloat(params, "intensity"),
                glm::normalize(extractVec3(params, "direction")),
                lookup_color(extractString(params, "color"))));
        } },

        { "point", [this](ParamMap& params) {
            lights.push_back(std::make_shared<PointLight>(
                extractFloat(params, "intensity"),
                extractVec3(params, "point"),
                lookup_color(extractString(params, "color"))));
        } },

        // Color
        { "color", [this](ParamMap& params) {
            std::string name = extractString(params, "name");
            colors.emplace(extractString(params, "name"), extractVec3(params, "value"));
        } },

        // Materials
        { "phong", [this](ParamMap& params) {
            std::string name = extractString(params, "name");

            auto material =
                std::make_shared<Phong>(extractFloat(params, "ka"),
                                        extractFloat(params, "kd"),
                                        extractFloat(params, "ks"),
                                        extractFloat(params, "ke"),
                                        extractFloat(params, "kr"),
                                        extractFloat(params, "kt"),
                                        extractFloat(params, "ki"));

            last_material = material;

            materials.insert(std::make_pair(std::move(name), material));
        } },

        // Samplers
        { "solid", [this](ParamMap& params) {
            last_material->sampler = std::make_shared<SolidSampler>(
                lookup_color(extractString(params, "color")));
        } },

        { "checkered", [this](ParamMap& params) {
            last_material->sampler = std::make_shared<CheckerboardSampler>(
                extractFloat(params, "size"),
                lookup_color(extractString(params, "color1")),
                lookup_color(extractString(params, "color2")));
        } },

        { "circles", [this](ParamMap& params) {
            last_material->sampler = std::make_shared<CirclesSampler>(
                extractFloat(params, "ring_size"),
                lookup_color(extractString(params, "color1")),
                lookup_color(extractString(params, "color2")));
        } },

        { "rainbow", [this](ParamMap& params) {
            last_material->sampler = std::make_shared<RainbowSampler>(
                lookup_color(extractString(params, "color1")),
                lookup_color(extractString(params, "color2")));
        } },

        { "png", [this](ParamMap& params) {
            auto texture = std::unique_ptr<PNGTexture>(new PNGTexture());
            texture->load(extractString(params, "filename"));

            last_material->sampler =
                std::make_shared<PNGTextureSampler>(std::move(texture));
        } },

        // Shapes
        { "sphere", [this](ParamMap& params) {
            auto materialName = extractString(params, "material");
            auto material = lookup_material(materialName);
            if (material == nullptr) {
                std::cerr << "WARNING: Material '" << materialName << "' not found!" << std::endl;
            }

            spatial_index->insert(std::make_shared<Sphere>(
                extractVec3(params, "point"),
                extractFloat(params, "radius"), material));
        } },

        { "triangle", [this](ParamMap& params) {
            auto materialName = extractString(params, "material");
            auto material = lookup_material(materialName);
            if (material == nullptr) {
                std::cerr << "WARNING: Material '" << materialName << "' not found!" << std::endl;
            }

            spatial_index->insert(std::make_shared<Triangle>(
                extractVec3(params, "p0"),
                extractVec2(params, "uv0"),
                extractVec3(params, "p1"),
                extractVec2(params, "uv1"),
                extractVec3(params, "p2"),
                extractVec2(params, "uv2"), material));
        } },

        { "obj_mesh", [this](ParamMap& params) {

            auto point = extractVec3(params, "point");
            auto rotate = extractVec3(params, "rotate");
            auto scale = extractFloat(params, "scale");

            auto materialName = extractString(params, "material");
            auto material = lookup_material(materialName);
            if (material == nullptr) {
                std::cerr << "WARNING: Material '" << materialName << "' not found!" << std::endl;
            }

            loadObj(extractString(params, "filename"), point, rotate, scale, material, spatial_index);
        } }
    };
}

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

std::string Kernel::toString(size_t depth) {
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;

    // Now we'll print out the pieces.
    ss << tabdepth << "CAMERAS: \n";
    for (CameraPtr& camera : cameras) {
        ss << camera->toString(depth + 1);
    }

    ss << tabdepth << "LIGHTS: \n";
    for (auto& light : lights) {
        ss << tabdepth << light->toString(depth + 1);
    }

    ss << tabdepth << "COLORS: \n";
    for (auto& color : colors) {
        ss << tabdepth << '\t' << color.first << ":\n";
        ss << tabdepth << "\t\t" << color.second << "\n";
    }

    ss << tabdepth << "MATERIALS: \n";
    for (auto& material : materials) {
        ss << tabdepth << '\t' << material.first << ":\n";
        ss << material.second->toString(depth + 2);
    }

    ss << tabdepth << "SPATIAL INDEX: \n";
    ss << spatial_index->toString(depth + 1);

    return ss.str();
}

struct SpawnRaysContext {
    Kernel* kernel;
    CameraPtr camera;
    size_t num_bounces;
};

void spawn_rays_cb(void* ct, size_t row, size_t col, Ray& ray) {
    SpawnRaysContext* ctx = (SpawnRaysContext*)ct;
    ctx->camera->view_plane->set_pixel(row, col, ctx->kernel->get_color_rec(ray, 1, ctx->num_bounces));
}

void Kernel::render() {
    typedef std::chrono::high_resolution_clock Clock;

    size_t i = 1;
    for (CameraPtr& camera : cameras) {
        auto start = Clock::now();
        
        // Give the camera a function to call on each ray it generates.
        SpawnRaysContext ctx{this, camera, camera->get_num_bounces()};
        camera->spawn_rays(&ctx, spawn_rays_cb);

        // Signal to the view plane that we are finished writing.
        camera->view_plane->finish();

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

    float kr = hit.shape->material->kr;
    float kt = hit.shape->material->kt;
    float ki = hit.shape->material->ki;

    if(ctx->num_bounces < ctx->max_bounces)
    {
        // Get Reflected Illumination
        if(kr > 0.0)
        {
            Ray refl{hit.intersection_point - hit.incoming_ray.direction * 0.05f, glm::reflect(hit.incoming_ray.direction, hit.intersection_normal)};
            refl.update();
            ctx->result += kr * ctx->kernel->get_color_rec(refl, ctx->num_bounces + 1, ctx->max_bounces);
        }

        // Get Transmitted Illumination
        if(kt > 0.0)
        {
            float dir = glm::dot(hit.intersection_normal, hit.incoming_ray.direction);

            auto normal = ((dir <= 0.0f) ? -1.0f : 1.0f) * hit.intersection_normal;

            auto refract1 = glm::refract( hit.incoming_ray.direction, normal, ctx->kernel->world_ki / ki );
            if(refract1 != glm::vec3(0.0f))
            {
                auto refract2 = glm::refract( refract1, -normal, ki / ctx->kernel->world_ki );
                if(refract2 != glm::vec3(0.0f))
                {
                    Ray trans{ hit.intersection_point + refract2 * 0.05f, refract2 };
                    trans.update();
                    ctx->result += kt * ctx->kernel->get_color_rec(trans, ctx->num_bounces + 1, ctx->max_bounces);
                }
                else
                {
                    Ray refl{hit.intersection_point - refract1 * 0.05f, glm::reflect(refract1, -normal)};
                    refl.update();
                    ctx->result += kt * ctx->kernel->get_color_rec(refl, ctx->num_bounces + 1, ctx->max_bounces);
                }
            }
            else
            {
                Ray refl{hit.intersection_point - hit.incoming_ray.direction * 0.05f, glm::reflect(hit.incoming_ray.direction, normal)};
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
