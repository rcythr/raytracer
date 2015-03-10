
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
    kdtree::dump<ShapePtr, AABB>(node, [&](size_t depth, AABB& aabb, size_t dim, float split_val) {
        switch(dim)
        {
        case 0:
            points.push_back(glm::vec3(split_val, aabb.min[1], aabb.min[2]));
            points.push_back(glm::vec3(split_val, aabb.min[1], aabb.max[2]));
            points.push_back(glm::vec3(split_val, aabb.max[1], aabb.max[2]));
            points.push_back(glm::vec3(split_val, aabb.max[1], aabb.min[2]));
            break;
        case 1:
            points.push_back(glm::vec3(aabb.min[0], split_val, aabb.min[2]));
            points.push_back(glm::vec3(aabb.min[0], split_val, aabb.max[2]));
            points.push_back(glm::vec3(aabb.max[0], split_val, aabb.max[2]));
            points.push_back(glm::vec3(aabb.max[0], split_val, aabb.min[2]));
            break;
        case 2:
            points.push_back(glm::vec3(aabb.min[0], aabb.min[1], split_val));
            points.push_back(glm::vec3(aabb.min[0], aabb.max[1], split_val));
            points.push_back(glm::vec3(aabb.max[0], aabb.max[1], split_val));
            points.push_back(glm::vec3(aabb.max[0], aabb.min[1], split_val));
            break;
        }
    });

    std::ofstream ss("kdout.obj");
    size_t o = 0;
    for(size_t i=0; i < points.size(); i += 4, ++o)
    {
        ss << "o obj" << o << '\n';
        for(size_t j=0; j < 4; ++j)
        {
            ss << "v " << points[i+j].x << " " << points[i+j].y << " " << points[i+j].z << "\n";
        }
        ss << "f " << (i+1) << ' ' << (i+2) << ' ' << (i+3) << ' ' << (i+4) << '\n';
    }
    ss.close();
}

void KDTreeSpatialIndex::find_closest_hit(
    const Ray& ray, std::function<void(HitResult&)> hit_callback) {
    kdtree::find_closest_hit<ShapePtr, AABB, Ray>(
        node, ray, [&](std::vector<ShapePtr> possible_hits) -> bool {
            HitResult best_result;
            HitResult result;
            for (auto obj : possible_hits) {
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

bool KDTreeSpatialIndex::has_hit(Ray& ray, ShapePtr omit_shape)
{
    bool result = false;
    kdtree::find_closest_hit<ShapePtr, AABB, Ray>(
        node, ray, [&] (std::vector<ShapePtr> possible_hits) -> bool {
            HitResult hit;
            for(auto obj : possible_hits)
            {
                if(obj != omit_shape)
                {
                    obj->test_hit(ray, hit);
                    if(hit.found_hit)
                    {
                        result = true;
                        return true;
                    }
                }
            }
            return false;
        });
    return result;
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

    ss << tabdepth << "TYPE: KDTREE\n";
    ss << tabdepth << "OBJECTS:\n";
    for (auto& obj : shapes) {
        ss << obj->toString(depth + 1);
    }

    return ss.str();
}
