#pragma once

#include "ray.hpp"

#include <glm/glm.hpp>

#define MIN(A, B) ((A <= B) ? A : B)
#define MAX(A, B) ((A >= B) ? A : B)

namespace raytracer {

struct AABB {
    AABB() {}

    AABB(const AABB& o) {
        min = o.min;
        max = o.max;
    }

    glm::vec3 center() const;

    glm::vec3 half_size() const;

    inline bool test_hit(const Ray& ray, float& tval) 
    {
        float t1 = (min.x - ray.origin.x)*ray.inv_direction.x;
        float t2 = (max.x - ray.origin.x)*ray.inv_direction.x;
        float t3 = (min.y - ray.origin.y)*ray.inv_direction.y;
        float t4 = (max.y - ray.origin.y)*ray.inv_direction.y;
        float t5 = (min.z - ray.origin.z)*ray.inv_direction.z;
        float t6 = (max.z - ray.origin.z)*ray.inv_direction.z;

        float tmin = MAX(MAX(MIN(t1, t2), MIN(t3, t4)), MIN(t5, t6));
        float tmax = MIN(MIN(MAX(t1, t2), MAX(t3, t4)), MAX(t5, t6));

        if(tmax < 0.0f || tmin > tmax)
        {
            tval = tmax;
            return false;
        }

        tval = tmin;
        return true;
    }

    glm::vec3 min, max;
};

AABB operator+(AABB& lhs, AABB& rhs);
}
