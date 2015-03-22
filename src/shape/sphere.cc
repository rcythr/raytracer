#include "shape/sphere.hpp"

#include "util/vec3_helpers.hpp"
#include "util/string_mult.hpp"

#include <iostream>
#include <sstream>
#include <math.h>

using namespace raytracer;

void Sphere::test_hit(const Ray& ray, HitResult& result) {
    // Major values for texting intersection
    auto oc = ray.origin - point;

    // b=2(l*(o-c))
    auto b = glm::dot(ray.direction, oc) * 2.0f;
    // c = (o-c)^2-r^2
    auto c = glm::dot(oc, oc) - radius * radius;
    // testVal = b^2-c;
    auto testVal = b * b - c * 4.0f;

    // if b^2-c >= 0
    if (testVal == 0.0) {
        float tval = -b / 2;
        if (tval >= 0.0f) {
            auto intersection_point = ray.origin + ray.direction * tval;

            // Calculate U-V coordinates.
            auto d_hat = glm::normalize(intersection_point - point);
            auto u = 0.5 + atan2(d_hat.z, d_hat.x) / (2 * M_PI);
            auto v = 0.5 - atan(d_hat.y) / M_PI;

            // First intersection
            result.hit(shared_from_this(), tval, u, v, intersection_point,
                       glm::normalize(intersection_point - point), ray);
            return;
        }
    } else if (testVal > 0.0) {
        float testValSqrt = sqrt(testVal);
        float tval;

        float tval1 = (-b / 2) + testValSqrt;
        float tval2 = (-b / 2) - testValSqrt;

        bool tval1gt0 = tval1 > 0.0f;
        bool tval2gt0 = tval2 > 0.0f;
        if (tval1gt0 && tval2gt0) {
            tval = std::min(tval1, tval2);
        } else if (tval1gt0 && !tval2gt0) {
            tval = tval1;
        } else if (!tval1gt0 && tval2gt0) {
            tval = tval2;
        } else {
            result.miss();
            return;
        }

        auto intersection_point = ray.origin + ray.direction * tval;

        // Calculate U-V coordinates.
        auto d_hat = glm::normalize(intersection_point - point);
        auto u = 0.5 + atan2(d_hat.z, d_hat.x) / (2 * M_PI);
        auto v = 0.5 - atan(d_hat.y) / M_PI;
        result.hit(shared_from_this(), tval, u, v, intersection_point,
                   glm::normalize(intersection_point - point), ray);
        return;
    }
    result.miss();
}

inline float squared(float v) { return v * v; }
bool Sphere::test_hit(const AABB& aabb) {
    float dist_squared = radius * radius;

    if (point.x < aabb.min[0])
        dist_squared -= squared(point.x - aabb.min[0]);
    else if (point.x > aabb.max[0])
        dist_squared -= squared(point.x - aabb.max[0]);

    if (point.y < aabb.min[1])
        dist_squared -= squared(point.y - aabb.min[1]);
    else if (point.y > aabb.max[1])
        dist_squared -= squared(point.y - aabb.max[1]);

    if (point.z < aabb.min[2])
        dist_squared -= squared(point.z - aabb.min[2]);
    else if (point.z > aabb.max[2])
        dist_squared -= squared(point.z - aabb.max[2]);

    return dist_squared > 0;
}

std::string Sphere::toString(size_t depth) {
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;

    ss << tabdepth << "TYPE: SPHERE\n";
    ss << tabdepth << "LOCATION: " << point << '\n';
    ss << tabdepth << "RADIUS: " << radius << '\n';
    ss << tabdepth << "MATERIAL: \n";
    ss << material->toString(depth + 1);

    return ss.str();
}
