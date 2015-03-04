#pragma once

#include "shape/shape.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Triangle : public Shape {
    Triangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2,
             std::shared_ptr<Material> mat)
        : Shape(mat), p0(p0), p1(p1), p2(p2) {
        aabb.min = glm::vec3(std::min(std::min(p0.x, p1.x), p2.x),
                             std::min(std::min(p0.y, p1.y), p2.y),
                             std::min(std::min(p0.z, p1.z), p2.z));

        aabb.max = glm::vec3(std::max(std::max(p0.x, p1.x), p2.x),
                             std::max(std::max(p0.y, p1.y), p2.y),
                             std::max(std::max(p0.z, p1.z), p2.z));
    }

    std::string toString(size_t depth = 0) override;

    void test_hit(const Ray& ray, HitResult& result) override;

    AABB& get_aabb() override { return aabb; }

    glm::vec3 p0, p1, p2;
    AABB aabb;
};
}
