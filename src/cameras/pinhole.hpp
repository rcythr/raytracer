#pragma once

#include "camera.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct PinholeCamera : public Camera {

    PinholeCamera(int hres, int vres, float pixel_size, int num_samples,
                  glm::vec3 point, glm::vec3 up, glm::vec3 look_at,
                  float view_distance) 
        : hres(hres)
        , vres(vres)
        , num_samples(num_samples)
        , pixel_size(pixel_size)
        , view_distance(view_distance)
        , point(point)
        , up(up)
        , look_at(look_at)
    {
    }

    ~PinholeCamera() override {}

    virtual std::string toString(size_t depth) override;

private:
    int hres, vres, num_samples;
    float pixel_size, view_distance;
    glm::vec3 point, up, look_at;
};
}
