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

    void find_closest_hit(Ray& ray, std::function<void(ShapePtr&, double)> hit_callback) override;

    std::string toString(size_t depth=0) override;
};

}
