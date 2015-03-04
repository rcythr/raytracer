#pragma once

#include <memory>

#include "AABB.hpp"
#include "ray.hpp"
#include "hit_result.hpp"

namespace raytracer {

struct Material;

struct Shape : std::enable_shared_from_this<Shape> {
    Shape(std::shared_ptr<Material> material) : material(material) {}

    virtual ~Shape() {}

    virtual std::string toString(size_t depth = 0) = 0;

    virtual void test_hit(const Ray& ray, HitResult& result) = 0;

    virtual AABB& get_aabb() = 0;

    std::shared_ptr<Material> material;
};

typedef std::shared_ptr<Shape> ShapePtr;
}
