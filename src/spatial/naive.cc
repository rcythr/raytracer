
#include "naive.hpp"

using namespace raytracer;

void NaiveSpatialIndex::insert(ShapePtr ptr) { shapes.push_back(ptr); }

void NaiveSpatialIndex::optimize() {}

void NaiveSpatialIndex::find_closest_hit(const Ray& ray, std::function<void(HitResult&)> hit_callback, ShapePtr omit_shape) {
    HitResult best_result;
    HitResult result;
    for (auto obj : shapes) {
        if(obj == omit_shape)
            continue;

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
    }
}

bool NaiveSpatialIndex::has_hit(Ray& ray, ShapePtr omit_shape) {
    HitResult hit;
    for (auto obj : shapes) {
        if (obj != omit_shape) {
            obj->test_hit(ray, hit);
            if (hit.found_hit) {
                return hit.shape->is_light == false;
            }
        }
    }
    return false;
}

void NaiveSpatialIndex::view_all_objects(std::function<void(ShapePtr&)> view_all_callback) { }
