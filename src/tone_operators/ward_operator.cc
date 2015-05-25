#include "tone_operators/ward_operator.hpp"
#include "util/log_avg_luminance.hpp"

using namespace raytracer;

void WardOperator::apply(ViewPlanePtr vp) {
    auto width = vp->get_width();
    auto height = vp->get_height();

    auto L_wa = log_avg_luminance(vp, l_max);
    auto sf = std::pow((1.219f + std::pow(100.f / 5.f, 0.4f)) / (1.219f + std::pow(L_wa, 0.4f)), 2.5f);

    for(int64_t i=0; i < height; ++i)
    {
        for(int64_t j=0; j < width; ++j)
        {
            vp->set_pixel(i, j, sf * vp->get_pixel(i, j));
        }
    }
}
