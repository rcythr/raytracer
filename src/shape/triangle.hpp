#pragma once

#include "shape/shape.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Triangle : public Shape {
    Triangle(glm::vec4 p0, glm::vec4 p1, glm::vec4 p2, std::shared_ptr<Material> mat)
        : Shape(mat), p0_(p0), p1_(p1), p2_(p2) {}

    std::string toString(size_t depth = 0) override;

    void test_hit(Ray& ray, HitResult& result) override;

    glm::vec4 p0() const { return viewTransform * p0_; }

    glm::vec4 p1() const { return viewTransform * p1_; }
    
    glm::vec4 p2() const { return viewTransform * p2_; }

private:
    glm::vec4 p0_, p1_, p2_;
};
}
