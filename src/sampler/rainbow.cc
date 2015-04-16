
#include "sampler/rainbow.hpp"

using namespace raytracer;

glm::vec3 RainbowSampler::get_color(float u, float v) const {
    return u * color1 + v * color2;
}
