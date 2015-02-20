
#include "kernel.hpp"

#include "util/split.hpp"
#include "util/xml_helpers.hpp"
#include "util/string_mult.hpp"
#include "util/vec3_helpers.hpp"
#include "util/obj_mesh.hpp"

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
        {"general", [this](ParamMap& params) {
            verbose = extractBool(params, "verbose", true);
            num_threads =
                extractInt(params, "num_threads",
                           std::max(std::thread::hardware_concurrency(), 1u));
            switch (extractInt(params, "strategy", 1)) {
                case 1:
                    trace_strategy = checkpoint1;
                    break;
                default:
                    throw std::runtime_error(
                        "Invalid trace strategy in config "
                        "file.");
                    break;
            }
        }},

        // Spatial Indecies
        {"naive_index", [this](ParamMap& params) {
            spatial_index = std::make_shared<NaiveSpatialIndex>();
        }},

        {"kdtree_index", [this](ParamMap& params) {
            // TODO:
            // spatial_index = std::make_shared<KDTreeSpatialIndex>();
        }},

        // Cameras
        {"pinhole", [this](ParamMap& params) {
            cameras.push_back(
                std::make_shared<PinholeCamera>(
                    extractFloat(params, "pixel_size", 0.50f),
                    extractInt(params, "num_samples", 1),
                    extractVec3(params, "eye", glm::vec3(0.0f, 0.0f, 0.0f)),
                    extractVec3(params, "up", glm::vec3(0.0f, 1.0f, 0.0f)),
                    extractVec3(params, "look_at", glm::vec3(0.0f, 0.0f, 1.0f)),
                    extractFloat(params, "view_distance", 600)
                )
            );
        }},
        
        // ViewPlanes
        {"ppm_viewplane", [this](ParamMap& params) {
            cameras.at(cameras.size()-1)->view_plane = std::make_shared<PPMViewPlane>(
                extractInt(params, "hres", 800),
                extractInt(params, "vres", 600),
                extractString(params, "filename", "out.ppm"));
        }},


        // Lights
        {"ambient", [this](ParamMap& params) {
            ambient_light = std::make_shared<AmbientLight>(
                extractFloat(params, "scale_radiance", 1.0f));
        }},

        {"directional", [this](ParamMap& params) {
            lights.push_back(std::make_shared<DirectionalLight>(
                extractVec3(params, "direction",
                            glm::vec3(100.0f, 100.0f, 100.f)),
                extractFloat(params, "scale_radiance", 3.0f)));
        }},

        // Color
        {"color", [this](ParamMap& params) {
            std::string name = extractString(params, "name", "");
            colors.emplace(
                extractString(params, "name", ""),
                extractVec3(params, "value", glm::vec3(0.0f, 0.0f, 0.0f)));
        }},

        // Materials
        {"matte", [this](ParamMap& params) {
            std::string name = extractString(params, "name", "");
            materials.insert(std::make_pair(
                std::move(name),
                std::make_shared<Matte>(
                    extractFloat(params, "ka", 0.25f),
                    extractFloat(params, "kd", 0.75f),
                    lookup_color(extractString(params, "color", "")))));
        }},

        // Shapes
        {"sphere", [this](ParamMap& params) {
            auto materialName = extractString(params, "material", "");
            auto material = lookup_material(materialName);
            if (material == nullptr) {
                std::cerr << "WARNING: Material '" << materialName
                          << "' not found!" << std::endl;
            }

            spatial_index->insert(std::make_shared<Sphere>(
                extractVec3(params, "point", glm::vec3(0.0f, 0.0f, 0.0f)),
                extractFloat(params, "radius", 0.0f), material));
        }},

        {"triangle", [this](ParamMap& params) {
            auto materialName = extractString(params, "material", "");
            auto material = lookup_material(materialName);
            if (material == nullptr) {
                std::cerr << "WARNING: Material '" << materialName
                          << "' not found!" << std::endl;
            }

            spatial_index->insert(
                std::make_shared<Triangle>(
                    extractVec3(params, "p0", glm::vec3(0.0f, 0.0f, 0.0f)),
                    extractVec3(params, "p1", glm::vec3(0.0f, 0.0f, 0.0f)),
                    extractVec3(params, "p2", glm::vec3(0.0f, 0.0f, 0.0f)),
                    material));
        }},

        {"objmesh", [this](ParamMap& params) {
            auto materialName = extractString(params, "material", "");
            auto material = lookup_material(materialName);
            if (material == nullptr) {
                std::cerr << "WARNING: Material '" << materialName
                          << "' not found!" << std::endl;
            }

            loadObj(extractString(params, "filename", ""), material, spatial_index);
        }}};
}

glm::vec3 Kernel::lookup_color(std::string name) {
    auto find_itr = colors.find(name);
    if (find_itr != colors.end()) {
        return find_itr->second;
    }
    throw std::runtime_error(std::string("Could not find color: '") + name +
                             "'");
}

MaterialPtr Kernel::lookup_material(std::string name) {
    auto find_itr = materials.find(name);
    if (find_itr != materials.end()) {
        return find_itr->second;
    }
    throw std::runtime_error(std::string("Could not find material: '") + name +
                             "'");
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
    for(CameraPtr& camera : cameras) 
    {
        ss << camera->toString(depth + 1);
    }

    ss << tabdepth << "LIGHTS: \n";
    ss << tabdepth << ambient_light->toString(depth + 1);
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
    
void Kernel::render() {
    typedef std::chrono::high_resolution_clock Clock;
    
    size_t i = 1;
    for(CameraPtr& c : cameras)
    {
        auto start = Clock::now();
        trace_strategy(this, c);
        auto end = Clock::now();
        
        std::cout << "Camera " << i << " Render Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
                  << " miliseconds" << std::endl;

        ++i;
    }
}
