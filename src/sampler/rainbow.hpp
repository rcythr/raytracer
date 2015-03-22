#pragma once

#include "sampler/sampler.hpp"

namespace raytracer {

struct RainbowSampler : public Sampler {

    RainbowSampler(glm::vec3 color1, glm::vec3 color2)
        : color1(color1), color2(color2) {}

    glm::vec3 get_color(float u, float v) const override;

    glm::vec3 color1, color2;
};
}
