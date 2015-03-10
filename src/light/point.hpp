#pragma once

#include "light/light.hpp"

namespace raytracer {

struct PointLight : public Light {
    PointLight(glm::vec3 point, glm::vec3 color)
        : Light(color)
        , point(point)
    {
    }

    virtual std::string toString(size_t depth = 0) override;

    virtual LightType get_type() override { return LightType::POINT; }

    glm::vec3 point;
};

}

