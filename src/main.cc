#include "types.hpp"
#include "util/xml.hpp"
#include "util/xml_helpers.hpp"
#include "util/split.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>
#include <functional>

using namespace raytracer;

struct Kernel
{
    CameraPtr camera;
    LightPtr ambient_light;
    std::vector<LightPtr> lights;
    std::unordered_map<std::string, ColorPtr> colors;
    std::unordered_map<std::string, MaterialPtr> materials;
    std::vector<ShapePtr> shapes;
    
    std::unordered_map<std::string, std::function<void(ParamMap&)>> handlers =
    {
        // Cameras
        {"pinhole", [this](ParamMap& params) {
            camera = std::make_shared<PinholeCamera>(
                extractInt(params, "hres", 400),
                extractInt(params, "vres", 400),
                extractFloat(params, "pixel_size", 0.50f),
                extractInt(params, "num_samples", 1),
                extractVec3(params, "eye", glm::vec3(0.0f, 0.0f, 0.0f)),
                extractVec3(params, "up", glm::vec3(0.0f, 1.0f, 0.0f)),
                extractVec3(params, "look_at", glm::vec3(0.0f, 0.0f, 1.0f)),
                extractFloat(params, "view_distance", 600));
        }},

        // Lights
        {"ambient", [this](ParamMap& params) {
            ambient_light = std::make_shared<AmbientLight>(
                extractFloat(params, "scale_radiance", 1.0f)
            );
        }},

        {"directional", [this](ParamMap& params) {
            lights.push_back(std::make_shared<DirectionalLight>(
                extractVec3(params, "direction", glm::vec3(100.0f, 100.0f, 100.f)),
                extractFloat(params, "scale_radiance", 3.0f)
            ));
        }},

        // Color
        {"color", [this](ParamMap& params) {
            std::string name = extractString(params, "name", "");
            auto value = std::make_shared<Color>(extractVec3(params, "value", glm::vec3(0.0f, 0.0f, 0.0f)));
            colors.insert(std::make_pair(std::move(name), std::move(value)));
        }},

        // Materials
        {"matte", [this](ParamMap& params) {
            std::string name = extractString(params, "name", "");
            materials.insert(std::make_pair(std::move(name),
                std::make_shared<Matte>(
                    extractFloat(params, "ka", 0.25f),
                    extractFloat(params, "kd", 0.75f),
                    lookup_color(extractString(params, "color", ""))
                )));
        }},

        // Shapes
        {"sphere", [this](ParamMap& params) {
            shapes.push_back(
                std::make_shared<Sphere>(
                    extractVec3(params, "point", glm::vec3(0.0f, 0.0f, 0.0f)),
                    extractFloat(params, "radius", 0.0f),
                    lookup_material(extractString(params, "material", ""))
                )
            );
        }},

        {"plane", [this](ParamMap& params) {
            shapes.push_back(
                std::make_shared<Plane>(
                    extractVec3(params, "point", glm::vec3(0.0f, 0.0f, 0.0f)),
                    extractVec3(params, "normal", glm::vec3(0.0f, 0.0f, 1.0f)),
                    lookup_material(extractString(params, "material", ""))
                )
            );
        }}
    };

    ColorPtr lookup_color(std::string name) 
    {
        auto find_itr = colors.find(name);
        if(find_itr != colors.end())
        {
            return find_itr->second;
        }
        return nullptr;
    }

    MaterialPtr lookup_material(std::string name) 
    {
        auto find_itr = materials.find(name);
        if(find_itr != materials.end())
        {
            return find_itr->second;
        }
        return nullptr;
    }

    void open(std::string& tag, ParamMap& params)
    {
        auto find_itr = handlers.find(tag);
        if(find_itr != handlers.end())
        {
            find_itr->second(params);
        }
    }

    void ascii(std::string& content) {}

    void close(std::string& tag) { }
};

int main(int argc, char* argv[])
{
    // Load the user settings into a kernel.
    Kernel kernel;
    loadXML("config.xml", kernel);

        

    return 0;
}

