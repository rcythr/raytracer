#pragma once

#include "spatial_index.hpp"

namespace raytracer
{

struct NaiveSpatialIndex : public SpatialIndex
{
private:
    std::vector<ShapePtr> objects;
public:

    void insert(ShapePtr ptr) override;

    std::vector<ShapePtr> get_possible_hits(Ray& ray) override;

    std::string toString(size_t depth=0) override;
};

}
