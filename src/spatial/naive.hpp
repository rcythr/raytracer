
#include "spatial/spatial_index.hpp"

namespace raytracer {

struct NaiveSpatialIndex : public SpatialIndex {

    virtual ~NaiveSpatialIndex() {}

    void insert(ShapePtr ptr) override;

    void optimize() override;

    virtual void find_closest_hit(
        const Ray& ray, std::function<void(HitResult&)> hit_callback, ShapePtr omit_shape = nullptr) override;

    bool has_hit(Ray& ray, ShapePtr omit_shape) override;

    void view_all_objects(std::function<void(ShapePtr&)> view_all_callback) override;

    std::vector<ShapePtr> shapes;

};

}
