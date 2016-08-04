#include "kernel.hpp"

#include "util/split.hpp"
#include "util/xml_helpers.hpp"
#include "util/vec3_helpers.hpp"
#include "util/obj_mesh.hpp"

#include <thread>

using namespace raytracer;

Kernel::Kernel() {
    handlers = {

        // General
        { "general", [this](ParamMap& params) {
            background_color = extractVec3(params, "background_color");
            num_threads = extractInt(params, "num_threads", std::max(std::thread::hardware_concurrency(), 1u));
            num_photons = extractInt(params, "num_photons", 0);
            
            final_gather_samples = extractInt(params, "final_gather_samples", 10);
            final_gather_bounces = extractInt(params, "final_gather_bounces", 3);

            global_knn = extractInt(params, "global_knn", 100);
            global_exposure = extractFloat(params, "global_exposure", 1.0f);

            caustic_knn = extractInt(params, "caustic_knn", 10);
            caustic_exposure = extractFloat(params, "caustic_exposure", 1.0f);
            
            world_ki = extractFloat(params, "world_ki", 1.0);
        } },

        // Spatial Indecies
        {"naive_index", [this](ParamMap& params) {
            spatial_index = std::make_shared<NaiveSpatialIndex>();    
        } },

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

        { "triangular", [this](ParamMap& params) {
            auto color = lookup_color(extractString(params, "color"));
            auto lightMat = std::make_shared<Lambertian>(color);
            auto shape = std::make_shared<Triangle>(
                extractVec3(params, "p0"), glm::vec2(0.0f),
                extractVec3(params, "p1"), glm::vec2(0.0f),
                extractVec3(params, "p2"), glm::vec2(0.0f),
                lightMat);
            shape->is_light = true;
            lights.push_back(std::make_shared<TriangularLight>(extractFloat(params, "intensity"), shape, color));
            spatial_index->insert(shape);
        } },

        { "spherical", [this](ParamMap& params) {
            auto color = lookup_color(extractString(params, "color"));
            auto lightMat = std::make_shared<Lambertian>(color);
            auto shape = std::make_shared<Sphere>(extractVec3(params, "point"), extractFloat(params, "radius"), lightMat);
            shape->is_light = true;
            lights.push_back(std::make_shared<SphericalLight>(extractFloat(params, "intensity"), shape, color));
            spatial_index->insert(shape);
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
                std::make_shared<Phong>(extractFloat(params, "ka", 0.0f),
                                        extractFloat(params, "kd"),
                                        extractFloat(params, "ks"),
                                        extractFloat(params, "ke"),
                                        extractFloat(params, "kt"),
                                        extractFloat(params, "ki"),
                                        extractBool(params, "is_hollow", false));

            last_material = material;

            materials.insert(std::make_pair(std::move(name), material));
        } },

        {"photon_mapped", [this](ParamMap& params) {
            std::string name = extractString(params, "name");

            auto material = std::make_shared<PhotonMapped>(extractFloat(params, "ka", 0.0f),
                                                           extractFloat(params, "kd"),
                                                           extractFloat(params, "ks"),
                                                           extractFloat(params, "ke"),
                                                           extractFloat(params, "kt"),
                                                           extractFloat(params, "ki"),
                                                           extractBool(params, "is_hollow", false));

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
        } },

        // Tone Operators
        {"naive", [this](ParamMap& params) {
            tone_operators.push_back(std::make_shared<LinearOperator>());
        } },

        {"gaussian", [this](ParamMap& params) { 
            tone_operators.push_back(std::make_shared<GaussianOperator>()); 
        } },

        {"ward", [this](ParamMap& params) { 
            tone_operators.push_back(std::make_shared<WardOperator>(extractFloat(params, "l_max"))); 
        } },
       
        {"reinhard", [this](ParamMap& params) { 
            tone_operators.push_back(std::make_shared<ReinhardOperator>(extractFloat(params, "l_max"))); 
        } }
    };
}

