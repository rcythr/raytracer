#include "checkerboard.hpp"

using namespace raytracer;

glm::vec3 CheckerboardSampler::get_color(float u, float v) const
{
    bool u_even = ((int)(u / check_size)) % 2 == 0;
    bool v_even = ((int)(v / check_size)) % 2 == 0;

    if(u_even ^ v_even) {
        return color1;
    } else {
        return color2;
    }
}
