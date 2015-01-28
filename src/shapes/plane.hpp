#pragma once

#include "shape.hpp"
#include "material.hpp"

#include <glm/glm.hpp>

namespace raytracer
{

struct Plane : public Shape
{
    Plane( glm::vec3 point, glm::vec3 normal, MaterialPtr material)
    {
    }
};

}
