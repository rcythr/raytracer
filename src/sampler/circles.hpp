#pragma once

#include "sampler/sampler.hpp"

namespace raytracer {

struct CirclesSampler : public Sampler {

    CirclesSampler(float ring_size, glm::vec3 color1, glm::vec3 color2)
        : ring_size(ring_size), color1(color1), color2(color2) {}

    glm::vec3 get_color(float u, float v) const override;

    float ring_size;
    glm::vec3 color1, color2;
};
}
