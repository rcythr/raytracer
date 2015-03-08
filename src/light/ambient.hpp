#pragma once

#include "light/light.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct AmbientLight : public Light {
    AmbientLight(float scale_radiance, glm::vec3 color) 
        : Light(color)
        , scale_radiance(scale_radiance) 
    {}

    ~AmbientLight() override {}

    std::string toString(size_t depth = 0) override;

    LightType get_type() { return LightType::AMBIENT; }

    float scale_radiance;
};
}
