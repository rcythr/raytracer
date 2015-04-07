#include "spatial/naive.hpp"

#include "util/string_mult.hpp"
#include <sstream>

using namespace raytracer;

void NaiveSpatialIndex::insert(ShapePtr ptr) { objects.push_back(ptr); }

void NaiveSpatialIndex::find_closest_hit(
    const Ray& ray, std::function<void(HitResult&)> hit_callback, ShapePtr omit_shape) {
    HitResult best_result;
    HitResult result;
    for (auto obj : objects) {
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
    HitResult result;
    for (auto obj : objects) {
        if (obj != omit_shape) {
            obj->test_hit(ray, result);
            if (result.found_hit)
                return true;
        }
    }
    return false;
}

void NaiveSpatialIndex::view_all_objects(
    std::function<void(ShapePtr&)> functor) {
    for (auto& obj : objects) {
        functor(obj);
    }
}

std::string NaiveSpatialIndex::toString(size_t depth) {
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;

    ss << tabdepth << "TYPE: NAIVE\n";
    ss << tabdepth << "OBJECTS:\n";
    for (auto& obj : objects) {
        ss << obj->toString(depth + 1);
    }

    return ss.str();
}
