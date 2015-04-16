#pragma once

#include <glm/glm.hpp>

namespace raytracer {

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
    glm::vec3 inv_direction;
    int x_sign, y_sign, z_sign;

    void update() {
        inv_direction =
            glm::vec3(1 / direction.x, 1 / direction.y, 1 / direction.z);
        x_sign = inv_direction.x < 0;
        y_sign = inv_direction.y < 0;
        z_sign = inv_direction.z < 0;
    }
};
}
