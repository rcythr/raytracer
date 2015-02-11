#pragma once

#include "light/light.hpp"

namespace raytracer {

struct AmbientLight : public Light {

    AmbientLight(float scale_radiance) 
        : scale_radiance(scale_radiance)
    {
    }

    ~AmbientLight() override {}

    std::string toString(size_t depth=0) override;

    float scale_radiance;
};
}
