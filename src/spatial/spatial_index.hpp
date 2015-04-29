#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "ray.hpp"
#include "hit_result.hpp"
#include "shape/shape.hpp"

namespace raytracer {

typedef void (*HitCallback)(void* ctx, HitResult&);
typedef void (*ViewAllCallback)(void* ctx, ShapePtr&);

struct SpatialIndex {
    virtual ~SpatialIndex() {}

    virtual void insert(ShapePtr ptr) = 0;

    virtual void optimize() {}

    virtual void find_closest_hit(
        const Ray& ray, HitCallback hit_callback, void* ctx = nullptr, ShapePtr omit_shape = nullptr) = 0;

    virtual bool has_hit(Ray& ray, ShapePtr omit_shape) = 0;

    virtual void view_all_objects(ViewAllCallback functor, void* ctx = nullptr) = 0;
};

typedef std::shared_ptr<SpatialIndex> SpatialIndexPtr;
}
