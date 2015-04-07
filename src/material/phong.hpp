#pragma once

#include "material/material.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Phong : public Material {

    Phong(float ka, float kd, float ks, float ke, float kr, float kt, float ki)
        : Material(kr, kt, ki), ka(ka), kd(kd), ks(ks), ke(ke) {}

    virtual ~Phong() override {}

    std::string toString(size_t depth = 0) override;

    glm::vec3 get_color(Kernel* kernel, HitResult& hit, size_t num_bounces, size_t max_bounces) override;

    float ka, kd, ks, ke, ki;
};
}
