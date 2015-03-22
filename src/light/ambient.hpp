#pragma once

#include "light/light.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct AmbientLight : public Light {
    AmbientLight(glm::vec3 color) : Light(color) {}

    ~AmbientLight() override {}

    std::string toString(size_t depth = 0) override;

    LightType get_type() { return LightType::AMBIENT; }
};
}
