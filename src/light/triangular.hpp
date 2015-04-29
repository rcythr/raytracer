#pragma once

#include "light/light.hpp"
#include "shape/triangle.hpp"

#include <random>

namespace raytracer {
    struct TriangularLight : public Light {
        TriangularLight(float intensity, std::shared_ptr<Triangle> triangle, glm::vec3 color)
            : Light(intensity, color)
            , triangle(triangle)
        {}

        virtual LightType get_type() override { return LightType::TRIANGULAR; }

		virtual std::vector<Ray> get_photons(int n);

        std::shared_ptr<Triangle> triangle;
    };
}
