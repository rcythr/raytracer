#pragma once

#include "light/light.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct DirectionalLight : public Light {
    DirectionalLight(float intensity, glm::vec3 direction, glm::vec3 color)
        : Light(intensity, color), direction(direction) {}

    ~DirectionalLight() override {}

    LightType get_type() { return LightType::DIRECTION; }

    glm::vec3 direction;
};
}
