#pragma once

#include "light/light.hpp"
#include "shape/triangle.hpp"

namespace raytracer {
    struct TriangularLight : public Light {
        TriangularLight(float intensity, std::shared_ptr<Triangle> triangle, glm::vec3 color)
            : Light(intensity, color)
            , triangle(triangle)
        {}

        virtual std::string toString(size_t depth = 0) override;

        virtual LightType get_type() override { return LightType::TRIANGULAR; }

        std::shared_ptr<Triangle> triangle;
    };
}
