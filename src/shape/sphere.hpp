#pragma once

#include "shape/shape.hpp"
#include "material/material.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Sphere : public Shape {
    Sphere(glm::vec3 point, float radius, MaterialPtr material)
        : Shape(material), point(point), radius(radius) {}

    ~Sphere() override {}

    void test_hit(Ray& ray, HitResult& result) override;

    std::string toString(size_t depth = 0) override;

    glm::vec3 point;
    float radius;
};
}
