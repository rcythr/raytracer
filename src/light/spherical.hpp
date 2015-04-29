#pragma once

#include "light/light.hpp"
#include "shape/sphere.hpp"

namespace raytracer {

    struct SphericalLight : public Light {
        SphericalLight(float intensity, std::shared_ptr<Sphere> sphere, glm::vec3 color)
            : Light(intensity, color)
            , sphere(sphere)
        {}

        virtual std::string toString(size_t depth = 0) override;

        virtual LightType get_type() override { return LightType::SPHERICAL; }

        std::shared_ptr<Sphere> sphere;
    };

}
