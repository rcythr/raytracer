#include "tone_operators/linear_operator.hpp"

using namespace raytracer;

void LinearOperator::apply(ViewPlanePtr view_plane)
{
    int64_t height = (int64_t)view_plane->get_height();
    int64_t width = (int64_t)view_plane->get_width();

    glm::vec3 min = view_plane->get_pixel(0, 0);
    glm::vec3 max = min;

    // First determine our range.
    for(int64_t i=0; i < height; ++i) {
        for(int64_t j=0; j < width; ++j) {
            auto v = view_plane->get_pixel(i, j);

            if(v.r < 0.0f) v.r = 0.0f;
            if(v.g < 0.0f) v.g = 0.0f;
            if(v.b < 0.0f) v.b = 0.0f;
            
            if(v.r > 1.0f) v.r = 1.0f;
            if(v.g > 1.0f) v.g = 1.0f;
            if(v.b > 1.0f) v.b = 1.0f;

            if(v.r < min.r) min.r = v.r;
            if(v.r > max.r) max.r = v.r;

            if(v.g < min.g) min.g = v.g;
            if(v.g > max.g) max.g = v.g;
            
            if(v.b < min.b) min.b = v.b;
            if(v.b > max.b) max.b = v.b;
        }
    }

    // Now calculate the linear equation for each color and scale
    for(int64_t i=0; i < height; ++i) {
        for(int64_t j=0; j < width; ++j) {
            auto v = view_plane->get_pixel(i, j);
            v.r /= max.r;
            v.g /= max.g;
            v.b /= max.b;
            view_plane->set_pixel(i, j, v);
        }
    }
}
