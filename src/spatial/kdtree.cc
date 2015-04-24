
#include "spatial/kdtree.hpp"
#include "util/string_mult.hpp"
#include "util/kdtree/kdtree.hpp"
#include <sstream>
#include <fstream>

using namespace raytracer;

void KDTreeSpatialIndex::insert(ShapePtr ptr) { shapes.push_back(ptr); }

void KDTreeSpatialIndex::optimize() {
    node = kdtree::create(shapes,
                          kdtree::policies::CutInHalf<ShapePtr, AABB>(3, 20));

    std::vector<glm::vec3> points;
    kdtree::dump<ShapePtr, AABB>(
        node, [&](size_t depth, AABB& aabb, size_t dim, float split_val) {
            switch (dim) {
                case 0:
                    points.push_back(
                        glm::vec3(split_val, aabb.min[1], aabb.min[2]));
                    points.push_back(
                        glm::vec3(split_val, aabb.min[1], aabb.max[2]));
                    points.push_back(
                        glm::vec3(split_val, aabb.max[1], aabb.max[2]));
                    points.push_back(
                        glm::vec3(split_val, aabb.max[1], aabb.min[2]));
                    break;
                case 1:
                    points.push_back(
                        glm::vec3(aabb.min[0], split_val, aabb.min[2]));
                    points.push_back(
                        glm::vec3(aabb.min[0], split_val, aabb.max[2]));
                    points.push_back(
                        glm::vec3(aabb.max[0], split_val, aabb.max[2]));
                    points.push_back(
                        glm::vec3(aabb.max[0], split_val, aabb.min[2]));
                    break;
                case 2:
                    points.push_back(
                        glm::vec3(aabb.min[0], aabb.min[1], split_val));
                    points.push_back(
                        glm::vec3(aabb.min[0], aabb.max[1], split_val));
                    points.push_back(
                        glm::vec3(aabb.max[0], aabb.max[1], split_val));
                    points.push_back(
                        glm::vec3(aabb.max[0], aabb.min[1], split_val));
                    break;
            }
        });

    std::ofstream ss("kdout.obj");
    size_t o = 0;
    for (size_t i = 0; i < points.size(); i += 4, ++o) {
        ss << "o obj" << o << '\n';
        for (size_t j = 0; j < 4; ++j) {
            ss << "v " << points[i + j].x << " " << points[i + j].y << " "
               << points[i + j].z << "\n";
        }
        ss << "f " << (i + 1) << ' ' << (i + 2) << ' ' << (i + 3) << ' '
           << (i + 4) << '\n';
    }
    ss.close();
}

struct CheckHitContext {
    const Ray& ray;
    HitCallback hit_callback;
    void* ctx;
    ShapePtr omit_shape;
};

bool check_hit_callback(void* ct, std::vector<ShapePtr>& possible_hits) {
    CheckHitContext* ctx = (CheckHitContext*)ct;
    HitResult best_result;
    HitResult result;
    for (auto obj : possible_hits) {
        if(obj == ctx->omit_shape)
            continue;

        result.found_hit = false;
        obj->test_hit(ctx->ray, result);
        if (result.found_hit) {
            if (best_result.found_hit == false ||
                result.tval < best_result.tval) {
                best_result = result;
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

bool has_hit_callback(void* ct, std::vector<ShapePtr>& possible_hits) {
    HasHitContext* ctx = (HasHitContext*)ct;
    HitResult hit;
    for (auto obj : possible_hits) {
        if (obj != ctx->omit_shape) {
            obj->test_hit(ctx->ray, hit);
            if (hit.found_hit) {
                ctx->result = true;
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

std::string KDTreeSpatialIndex::toString(size_t depth) {
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;

    ss << tabdepth << "TYPE: KDTREE\n";
    ss << tabdepth << "OBJECTS:\n";
    for (auto& obj : shapes) {
        ss << obj->toString(depth + 1);
    }

    return ss.str();
}
