#pragma once

#include "shape/shape.hpp"
#include "material/material.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Plane : public Shape {
    Plane(glm::vec4 point, glm::vec4 normal, MaterialPtr material)
        : Shape(material), point_(point), normal_(normal) {}

    void test_hit(Ray& ray, HitResult& result) override;

    std::string toString(size_t depth = 0) override;

    glm::vec4 point() const { return viewTransform * point_; }

    glm::vec4 normal() const { return glm::normalize(viewTransform * normal_); }

private:
    glm::vec4 point_, normal_;
};
}
