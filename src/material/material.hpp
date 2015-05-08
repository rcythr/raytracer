#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "hit_result.hpp"

#include "sampler/sampler.hpp"

namespace raytracer {

struct Kernel;

struct Material {
    Material(float kd, float ks, float kt, float ki, bool is_hollow) 
        : is_hollow(is_hollow)
        , kd(kd)
        , ks(ks)
        , kt(kt)
        , ki(ki) 
    { }

    virtual ~Material() {}

    virtual glm::vec3 get_color(Kernel* kernel, HitResult& hit) = 0;

    virtual glm::vec3 get_raw_color(HitResult& hit) {
        return sampler->get_color(hit.u, hit.v);
    }

    bool is_hollow;
    float kd; // Diffuse Reflectance coefficient;
    float ks; // Specular Reflectance coefficient.
    float kt; // Specular Transmission coefficient.
    float ki; // Index of refraction
    SamplerPtr sampler;
};

typedef std::shared_ptr<Material> MaterialPtr;
}
