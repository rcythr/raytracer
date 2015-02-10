#pragma once

#include <glm/glm.hpp>

namespace raytracer
{

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
};

}
