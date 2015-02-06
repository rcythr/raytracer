#pragma once

#include "light.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct DirectionalLight : public Light {

    DirectionalLight(glm::vec3 direction, float scale_radiance) 
        : direction(direction)
        , scale_radiance(scale_radiance)
    {
    }

    ~DirectionalLight() override {}

    virtual std::string toString(size_t depth=0) override;

    glm::vec3 direction;
    float scale_radiance;
};
}
