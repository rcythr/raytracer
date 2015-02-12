#pragma once

#include "shape/shape.hpp"
#include "material/material.hpp"

#include <glm/glm.hpp>

namespace raytracer
{

struct Rect : public Shape
{

    Rect(glm::vec3 point, glm::vec3 normal, 
            float width, float height, MaterialPtr material)
        : Shape(material)
        , point(point)
        , normal(normal)
        , width(width)
        , height(height)
    {}

    bool test_hit(Ray& ray, double& tmin) override;

    std::string toString(size_t depth=0) override;

    glm::vec3 point, normal;
    float width, height;
};

}
