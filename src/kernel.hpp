#pragma once

#include "types.hpp"
#include "util/xml.hpp"

#include "photon_map.hpp"

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
    size_t num_threads, num_photons, global_knn, caustic_knn, final_gather_samples, final_gather_bounces;
    glm::vec3 background_color;
    float world_ki, global_exposure, caustic_exposure;

    std::vector<CameraPtr> cameras;
    std::vector<LightPtr> lights;
    std::unordered_map<std::string, glm::vec3> colors;
    std::unordered_map<std::string, MaterialPtr> materials;
    SpatialIndexPtr spatial_index;

    PhotonMap caustic_photons;

    std::vector<ToneOperatorPtr> tone_operators;

    Kernel();

    // Called by XML library upon opening a tag.
    void open(std::string& tag, ParamMap& params);

    // Called by XML library for middle-of-tag content
    void ascii(std::string& content) {}

    // Called by XML library for the closing of a tag.
    void close(std::string& tag) {}

    void lightPass();

    void renderPass();

    glm::vec3 get_color_rec(const Ray& ray, size_t num_bounces, size_t max_bounces);
};
}
