
#include "spatial/spatial_index.hpp"

namespace kdtree
{
    struct KDNode;
}

namespace raytracer {

struct KDTreeSpatialIndex : public SpatialIndex {
    virtual ~KDTreeSpatialIndex() {}

    void insert(ShapePtr ptr) override;

    void optimize() override;

    void find_closest_hit(
        Ray& ray, std::function<void(HitResult&)> hit_callback) override;

    void view_all_objects(std::function<void(ShapePtr&)> functor) override;

    std::string toString(size_t depth = 0) override;

    std::vector<ShapePtr> shapes;
    std::shared_ptr<kdtree::KDNode> node;
};

}
