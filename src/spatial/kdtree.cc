
#include "spatial/kdtree.hpp"
#include "util/string_mult.hpp"
#include "util/kdtree/kdtree.hpp"
#include <sstream>

using namespace raytracer;

void KDTreeSpatialIndex::insert(ShapePtr ptr) { shapes.push_back(ptr); }

void KDTreeSpatialIndex::optimize() {
    node = kdtree::create(shapes,
                          kdtree::policies::CutInHalf<ShapePtr, AABB>(10, 10));
}

void KDTreeSpatialIndex::find_closest_hit(
    Ray& ray, std::function<void(HitResult&)> hit_callback) {
    kdtree::find_closest_hit<ShapePtr, AABB, Ray>(
        node, ray, [&](std::vector<ShapePtr> possible_hits) -> bool {
            HitResult best_result;
            HitResult result;
            for (auto obj : shapes) {
                result.found_hit = false;
                obj->test_hit(ray, result);
                if (result.found_hit) {
                    if (best_result.found_hit == false ||
                        result.tval < best_result.tval) {
                        best_result = result;
                    }
                }
            }

            if (best_result.found_hit) {
                hit_callback(best_result);
                return true;
            }
            return false;
        });
}

void KDTreeSpatialIndex::view_all_objects(
    std::function<void(ShapePtr&)> functor) {
    for (auto& s : shapes) {
        functor(s);
    }
}

std::string KDTreeSpatialIndex::toString(size_t depth) {
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;

    ss << tabdepth << "TYPE: NAIVE\n";
    ss << tabdepth << "OBJECTS:\n";
    for (auto& obj : shapes) {
        ss << obj->toString(depth + 1);
    }

    return ss.str();
}
