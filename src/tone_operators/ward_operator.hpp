#pragma once

#include "tone_operators/tone_operator.hpp"

namespace raytracer {

    struct WardOperator : public ToneOperator {

        WardOperator(float l_max) 
            : l_max(l_max)
        { }

        void apply(ViewPlanePtr view_plane) override;

        float l_max;
    };
}
