#pragma once

#include "material/material.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Phong : public Material {

    Phong(float ka, float kd, float ks, float ke, float kr, float kt, float ki, bool is_hollow)
        : Material(kr, kt, ki, is_hollow), ka(ka), kd(kd), ks(ks), ke(ke) {}

    virtual ~Phong() override {}

    glm::vec3 get_color(Kernel* kernel, HitResult& hit) override;

    float ka, kd, ks, ke, ki;
};
}
