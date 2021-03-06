#pragma once

#include "tone_operators/tone_operator.hpp"

namespace raytracer {

    struct LinearOperator : public ToneOperator {

        void apply(ViewPlanePtr view_plane) override;

    };

}
