#pragma once

#include "hit_result.hpp"

namespace raytracer {
struct Kernel;

struct Sampler {

    virtual ~Sampler() {}

    virtual glm::vec3 get_color(float u, float v) const = 0;
};
typedef std::shared_ptr<Sampler> SamplerPtr;
}
