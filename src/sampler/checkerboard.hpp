#pragma once

#include "sampler/sampler.hpp"

namespace raytracer {

struct CheckerboardSampler : public Sampler {

    CheckerboardSampler(float check_size, glm::vec3 color1, glm::vec3 color2)
        : check_size(check_size), color1(color1), color2(color2) {}

    glm::vec3 get_color(float u, float v) const override;

    float check_size;
    glm::vec3 color1, color2;
};
}
