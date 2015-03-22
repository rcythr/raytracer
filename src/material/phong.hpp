#pragma once

#include "material/material.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Phong : public Material {

    Phong(float ka, float kd, float ks, float ke) 
        : ka(ka)
        , kd(kd)
        , ks(ks)
        , ke(ke)
    {}

    virtual ~Phong() override {}

    std::string toString(size_t depth = 0) override;
    
    glm::vec3 get_color(Kernel* kernel, HitResult& hit) override;

    float ka, kd, ks, ke;
};

}