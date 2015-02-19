#pragma once

#include "spatial_index.hpp"

namespace raytracer {

struct NaiveSpatialIndex : public SpatialIndex {
   private:
    std::vector<ShapePtr> objects;

   public:
    void insert(ShapePtr ptr) override;

    void find_closest_hit(
        Ray& ray, std::function<void(HitResult&)> hit_callback) override;

    void view_all_objects(std::function<void(ShapePtr&)> functor) override;

    std::string toString(size_t depth = 0) override;
};
}
