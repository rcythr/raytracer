#pragma once

#include "material/material.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Matte : public Material {

    Matte(float ka, float kd, float ks, float ke, glm::vec3 color) 
        : ka_(ka)
        , kd_(kd)
        , ks_(ks)
        , ke_(ke)
        , color(color) 
    {}

    virtual ~Matte() override {}

    std::string toString(size_t depth = 0) override;

    glm::vec3 get_raw_color() override;

    float ka() override { return ka_; }

    float kd() override { return kd_; }

    float ks() override { return ks_; }

    float ke() override { return ke_; }

    float ka_, kd_, ks_, ke_;
    glm::vec3 color;
};

}
