#include "AABB.hpp"

#include "util/vec3_helpers.hpp"

using namespace raytracer;

enum class Quadrant { Left, Right, Middle };

bool AABB::test_hit(const Ray& ray, float& tval) {
    bool inside = true;
    Quadrant quadrant[3];
    float candidatePlane[3];

    for (int i = 0; i < 3; ++i) {
        if (ray.origin[i] < min[i]) {
            quadrant[i] = Quadrant::Left;
            candidatePlane[i] = min[i];
            inside = false;
        } else if (ray.origin[i] > max[i]) {
            quadrant[i] = Quadrant::Right;
            candidatePlane[i] = max[i];
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
        if (quadrant[i] != Quadrant::Middle && ray.direction[i] != 0.0f) {
            calcVal = (candidatePlane[i] - ray.origin[i]) / ray.direction[i];
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
            float coord = ray.origin[i] + tval * ray.direction[i];
            if (coord < min[i] || coord > max[i])
                return false;
        }
    }

    return true;
}

glm::vec3 AABB::center() const
{
    return glm::vec3( 
        (max[0] + min[0]) / 2.0f,
        (max[1] + min[1]) / 2.0f,
        (max[2] + min[2]) / 2.0f);
}

glm::vec3 AABB::half_size() const
{
    return glm::vec3( 
        (max[0] - min[0]) / 2.0f,
        (max[1] - min[1]) / 2.0f,
        (max[2] - min[2]) / 2.0f);
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
