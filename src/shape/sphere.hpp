#pragma once

#include "shape/shape.hpp"
#include "material/material.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Sphere : public Shape {
    Sphere(glm::vec3 point, float radius, MaterialPtr material)
        : Shape(material), point(point), radius(radius) {
        aabb.min =
            glm::vec3(point.x - radius, point.y - radius, point.z - radius);

        aabb.max =
            glm::vec3(point.x + radius, point.y + radius, point.z + radius);
    }

    ~Sphere() override {}

    void test_hit(Ray& ray, HitResult& result) override;

    std::string toString(size_t depth = 0) override;

    AABB& get_aabb() override { return aabb; }

    glm::vec3 point;
    float radius;
    AABB aabb;
};
}
