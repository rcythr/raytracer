
#include "spatial/spatial_index.hpp"

#include "util/kdtree/kdtree.hpp"

namespace raytracer {

struct KDTreeSpatialIndex : public SpatialIndex {
    virtual ~KDTreeSpatialIndex() {}

    void insert(ShapePtr ptr) override;

    void optimize() override;

    void find_closest_hit(const Ray& ray, std::function<void(HitResult&)> hit_callback, ShapePtr omit_shape = nullptr) override;

    bool has_hit(Ray& ray, ShapePtr omit_shape) override;

    void view_all_objects(std::function<void(ShapePtr&)> functor) override;

    std::string toString(size_t depth = 0) override;

    std::vector<ShapePtr> shapes;
    std::shared_ptr<kdtree::KDNode<ShapePtr, AABB> > node;
};
}
