#pragma once

#include "light/light.hpp"

namespace raytracer {

struct PointLight : public Light {
    PointLight(float intensity, glm::vec3 point, glm::vec3 color) : Light(intensity, color), point(point) {}

    virtual LightType get_type() override { return LightType::POINT; }

    glm::vec3 point;
};
}
