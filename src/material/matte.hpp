#pragma once

#include "material/material.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Matte : public Material {
    Matte(float ka, float kd, float ks, float ke, glm::vec3 color) : ka(ka), kd(kd), ks(ks), ke(ke), color(color) {}

    virtual ~Matte() override {}

    std::string toString(size_t depth = 0) override;

    glm::vec3 get_raw_color() override;

    float ka, kd, ks, ke;
    glm::vec3 color;
};

}
