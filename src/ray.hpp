#pragma once

#include <glm/glm.hpp>

namespace raytracer {

struct Ray {
    Ray()
    {
    }

    Ray(glm::vec3 origin, glm::vec3 direction)
        : origin(origin)
        , direction(direction)
    {
    }

    Ray(const Ray& other) 
    {
        this->origin = other.origin;
        this->direction = other.direction;
        this->inv_direction = other.inv_direction;
    }

    void update()
    {
        inv_direction.x = 1.0f / direction.x;
        inv_direction.y = 1.0f / direction.y;
        inv_direction.z = 1.0f / direction.z;
    }

    glm::vec3 origin;
    glm::vec3 direction;
    glm::vec3 inv_direction;
};
}
