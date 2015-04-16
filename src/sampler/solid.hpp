#pragma once

#include "sampler/sampler.hpp"

namespace raytracer {

struct SolidSampler : public Sampler {

    SolidSampler(glm::vec3 color) : color(color) {}

    glm::vec3 get_color(float u, float v) const override { return color; }

    glm::vec3 color;
};
}
