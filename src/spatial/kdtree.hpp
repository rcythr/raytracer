
#include "spatial/spatial_index.hpp"

#include "util/kdtree/kdtree.hpp"

namespace raytracer {

struct KDTreeSpatialIndex : public SpatialIndex {

    virtual ~KDTreeSpatialIndex() {}

    void insert(ShapePtr ptr) override;

    void optimize() override;

    void find_closest_hit(const Ray& ray, HitCallback hit_callback, void* ctx = nullptr, ShapePtr omit_shape = nullptr) override;

    bool has_hit(Ray& ray, ShapePtr omit_shape) override;

    void view_all_objects(ViewAllCallback functor, void* ctx = nullptr) override;

    std::vector<ShapePtr> shapes;
    std::shared_ptr<kdtree::KDNode<ShapePtr, AABB> > node;
};
}
