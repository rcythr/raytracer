#pragma once

#include "material/material.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Matte : public Material {
    Matte(float ka, float kd, glm::vec3 color) 
        : ka(ka)
        , kd(kd)
        , color(color)
    {
    }

    virtual ~Matte() override {}

    virtual std::string toString(size_t depth=0) override;

    float ka, kd;
    glm::vec3 color;
};
}
