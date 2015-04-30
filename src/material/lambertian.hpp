#pragma once

#include "material/material.hpp"

#include <glm/glm.hpp>

namespace raytracer {

    struct Lambertian : public Material {

        Lambertian(glm::vec3 color)
            : Material(0.0f, 0.0f, 0.0f, false)
            , color(color)
        {}

        glm::vec3 get_color(Kernel* kernel, HitResult& hit) override
        {
            return color;
        }

        glm::vec3 color;
    };
}
