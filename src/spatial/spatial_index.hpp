#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "ray.hpp"
#include "hit_result.hpp"
#include "shape/shape.hpp"

namespace raytracer
{

struct SpatialIndex 
{
    virtual ~SpatialIndex() {}

    virtual void insert(ShapePtr ptr) = 0;

    virtual void find_closest_hit(Ray& ray, std::function<void(HitResult&)> hit_callback) = 0;

    virtual std::string toString(size_t depth=0) = 0;
};

typedef std::shared_ptr<SpatialIndex> SpatialIndexPtr;

}
