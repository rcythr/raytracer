
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
    ss << tabdepth << "P_0: " << p0 << std::endl;
    ss << tabdepth << "P_1: " << p1 << std::endl;
    ss << tabdepth << "P_2: " << p2 << std::endl;
    ss << tabdepth << "MATERIAL: " << std::endl;
    ss << material->toString(depth + 1);
    return ss.str();
}

void Triangle::test_hit(const Ray& ray, HitResult& result) {
    glm::vec3 e1, e2;

    e1 = p1 - p0;
    e2 = p2 - p0;

    auto T = ray.origin - p0;
    auto P = glm::cross(ray.direction, e2);
    auto Q = glm::cross(T, e1);

    float p_dot_e1 = glm::dot(P, e1);

    if(p_dot_e1 == 0.0f)
    {
        result.miss();
        return;
    }

    float t = glm::dot(Q, e2) / p_dot_e1;
    float u = glm::dot(P, T) / p_dot_e1;
    float v = glm::dot(Q, ray.direction) / p_dot_e1;

    if(u < 0.0f || v < 0.0f || (u+v) > 1.0f)
    {
        result.miss();
        return;
    }

    result.hit(
        shared_from_this(),
        t,
        ((1.0f - u - v) * p0) + (u * p1) + (v * p2),
        glm::cross(e1, e2));
}
