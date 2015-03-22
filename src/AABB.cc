#include "AABB.hpp"

#include "util/vec3_helpers.hpp"

using namespace raytracer;

enum class Quadrant { Left, Right, Middle };

bool AABB::test_hit(const Ray& ray, float& tval) {
    glm::vec3 parameters[2];
    parameters[0] = min;
    parameters[1] = max;

    float txmin, txmax, tymin, tymax, tzmin, tzmax;
    txmin = (parameters[ray.x_sign].x - ray.origin.x) * ray.inv_direction.x;
    txmax = (parameters[1-ray.x_sign].x - ray.origin.x) * ray.inv_direction.x;
    tymin = (parameters[ray.y_sign].y - ray.origin.y) * ray.inv_direction.y;
    tymax = (parameters[1-ray.y_sign].y - ray.origin.y) * ray.inv_direction.y;

    if( (txmin > tymax) || (tymin > txmax) )
        return false;
    if(tymin > txmin)
        txmin = tymin;
    if(tymax < txmax)
        txmax = tymax;

    tzmin = (parameters[ray.z_sign].z - ray.origin.z) * ray.inv_direction.z;
    tzmax = (parameters[1-ray.z_sign].z - ray.origin.z) * ray.inv_direction.z;

    if( (txmin > tzmax) || (tzmin > txmax) )
        return false;
    if(tzmin > txmin)
        txmin = tzmin;
    if(tzmax < txmax)
        txmax = tzmax;

    tval = txmax;
    return (txmax > 0.0f);
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
