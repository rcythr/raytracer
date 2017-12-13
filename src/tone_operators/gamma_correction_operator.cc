#include "tone_operators/gamma_correction_operator.hpp"

using namespace raytracer;

void GammaCorrectionOperator::apply(ViewPlanePtr vp) {
  int64_t height = (int64_t)vp->get_height();
  int64_t width = (int64_t)vp->get_width();

  float correction = 1.0f / gamma;

  for (int64_t i = 0; i < (int64_t)height; ++i) {
      for (int64_t j = 0; j < (int64_t)width; ++j) {
          auto v = vp->get_pixel(i, j);

          v.r = 255.0f * powf(v.r / 255.0f, correction);
          v.g = 255.0f * powf(v.g / 255.0f, correction);
          v.b = 255.0f * powf(v.b / 255.0f, correction);

          vp->set_pixel(i, j, v);
      }
  }
}
