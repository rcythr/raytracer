#pragma once

#include "types.hpp"
#include "util/xml.hpp"
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include <functional>

namespace raytracer {

struct Kernel {
  private:
    std::unordered_map<std::string, std::function<void(ParamMap&)> > handlers;

    // Lookup helper functions
    glm::vec3 lookup_color(std::string name);
    MaterialPtr lookup_material(std::string name);

    MaterialPtr last_material;

  public:
    bool verbose;
    size_t num_threads;
    glm::vec3 background_color;

    std::vector<CameraPtr> cameras;
    std::vector<LightPtr> lights;
    std::unordered_map<std::string, glm::vec3> colors;
    std::unordered_map<std::string, MaterialPtr> materials;
    SpatialIndexPtr spatial_index;
    std::function<void(Kernel*, CameraPtr)> trace_strategy;

    Kernel();

    // Called by XML library upon opening a tag.
    void open(std::string& tag, ParamMap& params);

    // Called by XML library for middle-of-tag content
    void ascii(std::string& content) {}

    // Called by XML library for the closing of a tag.
    void close(std::string& tag) {}

    // Usual to string function
    std::string toString(size_t depth = 0);

    void render();
};
}
