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

    virtual std::string toString(size_t depth = 0) = 0;

    virtual glm::vec3 get_color(Kernel* kernel, HitResult& hit, size_t num_bounces, size_t max_bounces) = 0;

    float kr; // Reflectance coefficient.
    float kt; // Transmission coefficient.
    float ki; // Index of refraction
    SamplerPtr sampler;
};

typedef std::shared_ptr<Material> MaterialPtr;
}
