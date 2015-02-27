#pragma once

#include "ray.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct AABB {
    AABB() {}

    AABB(const AABB& o) {
        min = o.min;
        max = o.max;
    }

    bool test_hit(Ray& r);

    glm::vec3 min, max;
};

AABB operator+(AABB& lhs, AABB& rhs);
}
