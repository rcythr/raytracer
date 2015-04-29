#include "AABB.hpp"

#include "util/vec3_helpers.hpp"

using namespace raytracer;

enum class Quadrant { Left, Right, Middle };

glm::vec3 AABB::center() const {
    return glm::vec3((max[0] + min[0]) / 2.0f, (max[1] + min[1]) / 2.0f,
                     (max[2] + min[2]) / 2.0f);
}

glm::vec3 AABB::half_size() const {
    return glm::vec3((max[0] - min[0]) / 2.0f, (max[1] - min[1]) / 2.0f,
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
