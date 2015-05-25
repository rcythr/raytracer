#include "tone_operators/ward_operator.hpp"

using namespace raytracer;

float luminance(glm::vec3 pixel) {
    return 0.27f * pixel.r + 0.67f * pixel.g + 0.06f * pixel.b;
}

float log_avg_luminance(ViewPlanePtr vp) {
    int64_t width = vp->get_width();
    int64_t height = vp->get_height();

    float result = 0.0f;
    for(int64_t i=0; i < width; ++i)
    {
        for(int64_t j=0; j < height; ++j)
        {
            result += std::log(0.0001f + luminance(vp->get_pixel(i, j)));
        }
    }
    return std::exp(result / (width * height));
}

void WardOperator::apply(ViewPlanePtr vp) {
    float L_avg = log_avg_luminance(vp);
    float sf = 1.219f + pow(l_max / 2.0f, 0.4f);
    sf /= 1.219f + pow(L_avg, 0.4f);
    sf = std::pow(sf, 2.5);

    int64_t width = vp->get_width();
    int64_t height = vp->get_height();
    for(int64_t i=0; i < width; ++i)
    {
        for(int64_t j=0; j < height; ++j)
        {
            vp->set_pixel(i, j, sf * vp->get_pixel(i, j));
        }
    }
}
