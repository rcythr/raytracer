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

    bool test_hit(const Ray& r, float& tval);

    glm::vec3 min, max;
};

AABB operator+(AABB& lhs, AABB& rhs);
}
