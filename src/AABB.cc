#include "AABB.hpp"

#include "util/vec3_helpers.hpp"

using namespace raytracer;

enum class Quadrant { Left, Right, Middle };

bool AABB::test_hit(const Ray& ray, float& tval) {
    bool inside = true;
    Quadrant quadrant[3];
    float candidatePlane[3];

    for (int i = 0; i < 3; ++i) {
        if (val(ray.origin, i) < val(min, i)) {
            quadrant[i] = Quadrant::Left;
            candidatePlane[i] = val(min, i);
            inside = false;
        } else if (val(ray.origin, i) > val(max, i)) {
            quadrant[i] = Quadrant::Right;
            candidatePlane[i] = val(max, i);
            inside = false;
        } else {
            quadrant[i] = Quadrant::Middle;
        }
    }

    if (inside) {
        return true;
    }

    // Find the maximum intersection
    int whichPlane;
    bool minSet = false;
    for (int i = 0; i < 3; ++i) {
        float calcVal;
        if (quadrant[i] != Quadrant::Middle && val(ray.direction, i) != 0.0f) {
            calcVal = (candidatePlane[i] - val(ray.origin, i)) /
                      val(ray.direction, i);
        } else {
            calcVal = -1.0f;
        }

        if (!minSet || calcVal < tval) {
            if(tval > 0.0f)
            {
                tval = calcVal;
                whichPlane = i;
                minSet = true;
            }
        }
    }

    // If the intersection is behind the ray's origin, no intersection
    if (!minSet)
        return false;

    // Perform the final check of the condidate.
    for (int i = 0; i < 3; ++i) {
        if (whichPlane != i) {
            float coord = val(ray.origin, i) + tval * val(ray.direction, i);
            if (coord < min[i] || coord > max[i])
                return false;
        }
    }

    return true;
}

AABB raytracer::operator+(AABB& lhs, AABB& rhs) {
    AABB result;
    result.min = glm::vec3(std::min(lhs.min.x, rhs.min.x),
                           std::min(lhs.min.y, rhs.min.y),
                           std::min(lhs.min.z, rhs.min.z));

    result.max = glm::vec3(std::max(lhs.max.x, rhs.max.x),
                           std::max(lhs.max.y, rhs.max.y),
                           std::max(lhs.max.z, rhs.max.z));

    return result;
}
