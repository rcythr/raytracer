#pragma once

#include "camera/camera.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct PinholeCamera : public Camera {

    PinholeCamera(float pixel_size, int num_samples,
                  glm::vec3 point, glm::vec3 up, glm::vec3 look_at,
                  float view_distance) 
        : num_samples(num_samples)
        , pixel_size(pixel_size)
        , view_distance(view_distance)
        , point(point)
        , up(up)
        , look_at(look_at)
    {
    }

    ~PinholeCamera() override {}

    void spawn_rays(std::function<void(size_t, size_t, Ray&)> spawn_callback) override;

    std::string toString(size_t depth) override;

private:
    int num_samples;
    float pixel_size, view_distance;
    glm::vec3 point, up, look_at;
};
}
