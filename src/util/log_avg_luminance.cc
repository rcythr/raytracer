#include "log_avg_luminance.hpp"

#include "view_plane/view_plane.hpp"

using namespace raytracer;

float raytracer::luminance(const glm::vec3 color) { return 0.27f * color.r + 0.67f * color.g + 0.06f * color.b; }

float raytracer::avg_luminance(std::shared_ptr<ViewPlane>& vp, float l_max) {
    auto width = vp->get_width();
    auto height = vp->get_height();

    float result = 0.0f;
    for(int64_t i=0; i < height; ++i)
    {
        for(int64_t j=0; j < width; ++j)
        {
            result += luminance(vp->get_pixel(i, j) * l_max);
        }
    }

    return result / (float)(width * height);
}

float raytracer::log_avg_luminance(std::shared_ptr<ViewPlane>& vp, float l_max) {
    auto width = vp->get_width();
    auto height = vp->get_height();

    float result = 0.0f;
    for(int64_t i=0; i < height; ++i)
    {
        for(int64_t j=0; j < width; ++j)
        {
            result += std::log(0.001f + luminance(vp->get_pixel(i, j)) * l_max);
        }
    }

    return std::exp(result / (float)(width * height));
}

