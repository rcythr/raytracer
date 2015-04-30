
#include "spatial/spatial_index.hpp"

namespace raytracer {

struct NaiveSpatialIndex : public SpatialIndex {

    virtual ~NaiveSpatialIndex() {}

    void insert(ShapePtr ptr) override;

    void optimize() override;

    void find_closest_hit(const Ray& ray, HitCallback hit_callback, void* ctx = nullptr, ShapePtr omit_shape = nullptr) override;

    bool has_hit(Ray& ray, ShapePtr omit_shape) override;

    void view_all_objects(ViewAllCallback functor, void* ctx = nullptr) override;

    std::vector<ShapePtr> shapes;

};

}
