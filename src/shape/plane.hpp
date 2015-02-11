#pragma once

#include "shape/shape.hpp"
#include "material/material.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Plane : public Shape {
    Plane(glm::vec3 point, glm::vec3 normal, MaterialPtr material) 
        : point(point)
        , normal(normal)
        , material(material)
    {
    }

    ~Plane() override {}

    std::string toString(size_t depth=0) override;

    glm::vec3 point;
    glm::vec3 normal;
    MaterialPtr material;
};
}
