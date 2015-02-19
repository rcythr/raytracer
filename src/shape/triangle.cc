
#include "shape/triangle.hpp"
#include "material/material.hpp"

#include <sstream>
#include "util/string_mult.hpp"
#include "util/vec3_helpers.hpp"

using namespace raytracer;

std::string Triangle::toString(size_t depth) {
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;
    ss << tabdepth << "TYPE: TRIANGLE" << std::endl;
    ss << tabdepth << "P_0: " << p0_ << std::endl;
    ss << tabdepth << "P_1: " << p1_ << std::endl;
    ss << tabdepth << "P_2: " << p2_ << std::endl;
    ss << tabdepth << "MATERIAL: " << std::endl;
    ss << material->toString(depth + 1);
    return ss.str();
}

void Triangle::test_hit(Ray& ray, HitResult& result) {

    auto vP0 = glm::vec3(p0());
    auto vP1 = glm::vec3(p1());
    auto vP2 = glm::vec3(p2());

    float a, f, u, v, t;
    glm::vec3 e1, e2, h, s, q;

    e1 = vP1 - vP0;
    e2 = vP2 - vP0;

    h = glm::cross(ray.direction, e2);
    a = glm::dot(e1, h);

    if (a > -0.00001 && a < 0.00001) {
        result.miss();
        return;
    }

    f = 1 / a;

    s = ray.origin - vP0;
    u = f * glm::dot(s, h);

    if (u < 0.0 || u > 1.0) {
        result.miss();
        return;
    }

    q = glm::cross(s, e1);

    v = f * glm::dot(ray.direction, q);

    if (v < 0.0 || u + v > 1.0) {
        result.miss();
        return;
    }

    t = f * glm::dot(e2, q);

    if (t > 0.00001) {
        result.hit(shared_from_this(), t, ray.origin + ray.direction * t,
                   glm::cross(e1, e2));
        return;
    }
    result.miss();
}
