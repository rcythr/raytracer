#pragma once

#include "tone_operators/tone_operator.hpp"

namespace raytracer {

    struct GammaCorrectionOperator : public ToneOperator {

        GammaCorrectionOperator(float gamma) : gamma(gamma) {}

        void apply(ViewPlanePtr view_plane) override;

        float gamma;
    };

}
