#pragma once

#include "light/light.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct AmbientLight : public Light {
    AmbientLight(float scale_radiance, glm::vec3 color) : scale_radiance(scale_radiance), color(color) {}

    ~AmbientLight() override {}

    std::string toString(size_t depth = 0) override;

    float scale_radiance;
    glm::vec3 color;
};
}
