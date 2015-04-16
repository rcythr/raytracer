#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "ray.hpp"
#include "hit_result.hpp"
#include "shape/shape.hpp"

namespace raytracer {

struct SpatialIndex {
    virtual ~SpatialIndex() {}

    virtual void insert(ShapePtr ptr) = 0;

    virtual void optimize() {}

    virtual void find_closest_hit(
        const Ray& ray, std::function<void(HitResult&)> hit_callback, ShapePtr omit_shape = nullptr) = 0;

    virtual bool has_hit(Ray& ray, ShapePtr omit_shape) = 0;

    virtual void view_all_objects(std::function<void(ShapePtr&)> functor) = 0;

    virtual std::string toString(size_t depth = 0) = 0;
};

typedef std::shared_ptr<SpatialIndex> SpatialIndexPtr;
}
