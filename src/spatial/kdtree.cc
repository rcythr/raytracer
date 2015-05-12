
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

struct CheckHitContext {
    const Ray& ray;
    HitCallback hit_callback;
    void* ctx;
    ShapePtr omit_shape;
};

bool check_hit_callback(void* ct, AABB& bounds,  std::vector<ShapePtr>& possible_hits) {
    CheckHitContext* ctx = (CheckHitContext*)ct;
    HitResult best_result;
    HitResult result;
    for (auto obj : possible_hits) {
        if(obj == ctx->omit_shape)
            continue;

        result.found_hit = false;
        obj->test_hit(ctx->ray, result);
        if (result.found_hit) {
            if (best_result.found_hit == false || result.tval < best_result.tval) {
                if(bounds.contains(result.intersection_point)) {
                    best_result = result;
                }
            }
        }
    }

    if (best_result.found_hit) {
        ctx->hit_callback(ctx->ctx, best_result);
        return true;
    }
    return false;
}

void KDTreeSpatialIndex::find_closest_hit(const Ray& ray, HitCallback hit_callback, void* ctx, ShapePtr omit_shape) {
    CheckHitContext sub_ctx{ray, hit_callback, ctx, omit_shape};
    kdtree::find_closest_hit<ShapePtr, AABB, Ray>(node, ray, check_hit_callback, &sub_ctx);
}

struct HasHitContext {
    bool& result;
    Ray& ray;
    ShapePtr omit_shape;
};

bool has_hit_callback(void* ct, AABB& bounds, std::vector<ShapePtr>& possible_hits) {
    HasHitContext* ctx = (HasHitContext*)ct;
    HitResult hit;
    for (auto obj : possible_hits) {
        if (obj != ctx->omit_shape) {
            obj->test_hit(ctx->ray, hit);
            if (hit.found_hit && bounds.contains(hit.intersection_point)) {
                ctx->result = hit.shape->is_light == false;
                return true;
            }
        }
    }
    return false;
}

bool KDTreeSpatialIndex::has_hit(Ray& ray, ShapePtr omit_shape) {
    bool result = false;
    HasHitContext ctx{result, ray, omit_shape};
    kdtree::find_closest_hit<ShapePtr, AABB, Ray>( node, ray, has_hit_callback, &ctx);
    return result;
}

void KDTreeSpatialIndex::view_all_objects(ViewAllCallback functor, void* ctx) {
    for (auto& s : shapes) {
        functor(ctx, s);
    }
}

