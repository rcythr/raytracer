#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "hit_result.hpp"

#include "sampler/sampler.hpp"

namespace raytracer {

struct Kernel;

struct Material {
    virtual ~Material() {}

    virtual std::string toString(size_t depth = 0) = 0;

    virtual glm::vec3 get_color(Kernel* kernel, HitResult& hit) = 0;

    SamplerPtr sampler;
};

typedef std::shared_ptr<Material> MaterialPtr;
}
