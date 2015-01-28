#pragma once

#include "light.hpp"

namespace raytracer
{

struct AmbientLight : public Light
{

    AmbientLight(float scale_radiance)
    {
    }

};

}
