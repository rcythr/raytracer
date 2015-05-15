#include "tone_operators/reinhard_operator.hpp"
#include "util/log_avg_luminance.hpp"

using namespace raytracer;

void ReinhardOperator::apply(ViewPlanePtr vp) {
    auto width = vp->get_width();
    auto height = vp->get_height();

    auto L_wa = avg_luminance(vp, l_max);
    
    float a_lw = 0.18f / L_wa;
    for(int64_t i=0; i < height; ++i)
    {
        for(int64_t j=0; j < width; ++j)
        {
            auto color = vp->get_pixel(i, j) * l_max;

            color.r = a_lw * color.r;
            color.g = a_lw * color.g;
            color.b = a_lw * color.b;
            
            color.r = color.r / (1.0f + color.r);
            color.g = color.g / (1.0f + color.g);
            color.b = color.b / (1.0f + color.b);

            vp->set_pixel(i, j, color);
        }
    }
}
