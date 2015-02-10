#pragma once

#include <memory>
#include <vector>

#include "ray.hpp"
#include "shape.hpp"

namespace raytracer
{

struct SpatialIndex 
{
    virtual ~SpatialIndex() {}

    virtual void insert(ShapePtr ptr) = 0;

    virtual std::vector<ShapePtr> get_possible_hits(Ray& ray) = 0;

    virtual std::string toString(size_t depth=0) = 0;
};

typedef std::shared_ptr<SpatialIndex> SpatialIndexPtr;

}
