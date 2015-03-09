#pragma once

#include "light/light.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct DirectionalLight : public Light {
    DirectionalLight(glm::vec3 direction, glm::vec3 color)
        : Light(color)
        , direction(direction)
        {}

    ~DirectionalLight() override {}

    virtual std::string toString(size_t depth = 0) override;

    LightType get_type() { return LightType::DIRECTION; }

    glm::vec3 direction;
};
}
