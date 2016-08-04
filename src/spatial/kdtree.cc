
#include "spatial/kdtree.hpp"
#include "util/kdtree/kdtree.hpp"
#include <sstream>
#include <fstream>

using namespace raytracer;

void KDTreeSpatialIndex::insert(ShapePtr ptr) { shapes.push_back(ptr); }

void KDTreeSpatialIndex::optimize() {
    node = kdtree::create(shapes,
                          kdtree::policies::CutInHalf<ShapePtr, AABB>(3, 20));
}

void KDTreeSpatialIndex::find_closest_hit(const Ray& ray, std::function<void(HitResult&)> hit_callback, ShapePtr omit_shape) {
    kdtree::find_closest_hit<ShapePtr, AABB, Ray>(node, ray, 
        [&](AABB& bounds, std::vector<ShapePtr>& possible_hits){
            HitResult best_result;
            HitResult result;
            for (auto obj : possible_hits) {
                if(obj == omit_shape)
                    continue;

                result.found_hit = false;
                obj->test_hit(ray, result);
                if (result.found_hit) {
                    if (best_result.found_hit == false || result.tval < best_result.tval) {
                        if(bounds.contains(result.intersection_point)) {
                            best_result = result;
                        }
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

bool KDTreeSpatialIndex::has_hit(Ray& ray, ShapePtr omit_shape) {
    bool result = false;
    kdtree::find_closest_hit<ShapePtr, AABB, Ray>( node, ray, [&](AABB& bounds, std::vector<ShapePtr>& possible_hits){
        HitResult hit;
        for (auto obj : possible_hits) {
            if (obj != omit_shape) {
                obj->test_hit(ray, hit);
                if (hit.found_hit && bounds.contains(hit.intersection_point)) {
                    result = hit.shape->is_light == false;
                    return true;
                }
            }
        }
        return false;
    });
    return result;
}

void KDTreeSpatialIndex::view_all_objects(std::function<void(ShapePtr&)> view_all_callback) {
    for (auto& s : shapes) {
        view_all_callback(s);
    }
}

