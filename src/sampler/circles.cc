#include "circles.hpp"

using namespace raytracer;

glm::vec3 CirclesSampler::get_color(float u, float v) const
{
    double distance = sqrt(pow(u - 0.5, 2) + pow(v - 0.5, 2));
    if(int(distance / ring_size) % 2 == 0) {
        return color1;
    } else { 
        return color2;
    }
}
