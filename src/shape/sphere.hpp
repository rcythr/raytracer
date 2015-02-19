#pragma once

#include "shape/shape.hpp"
#include "material/material.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Sphere : public Shape {
    Sphere(glm::vec4 point, float radius, MaterialPtr material)
        : Shape(material), point_(point), radius_(radius) {}

    ~Sphere() override {}

    void test_hit(Ray& ray, HitResult& result) override;

    std::string toString(size_t depth = 0) override;

    glm::vec4 point() const { return viewTransform * point_; }

    float radius() const { return radius_; }

private:
    glm::vec4 point_;
    float radius_;
};
}
