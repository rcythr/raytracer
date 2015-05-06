#pragma once

#include <memory>
#include "view_plane/view_plane.hpp"

namespace raytracer {

    struct ToneOperator {
        virtual void apply(std::shared_ptr<ViewPlane> vp) = 0; 
    };

    typedef std::shared_ptr<ToneOperator> ToneOperatorPtr;
}
