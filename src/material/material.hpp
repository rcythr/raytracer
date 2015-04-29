#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "hit_result.hpp"

#include "sampler/sampler.hpp"

namespace raytracer {

struct Kernel;

struct Material {
    Material(float kr, float kt, float ki) : kr(kr), kt(kt), ki(ki) {}

    virtual ~Material() {}

    virtual glm::vec3 get_color(Kernel* kernel, HitResult& hit) = 0;

    float kr; // Reflectance coefficient.
    float kt; // Transmission coefficient.
    float ki; // Index of refraction
    SamplerPtr sampler;
};

typedef std::shared_ptr<Material> MaterialPtr;
}
