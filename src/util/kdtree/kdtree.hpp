#pragma once

#include <tuple>
#include "util/vec3_helpers.hpp"

using namespace raytracer;

namespace kdtree {

enum class Type { INNER, LEAF };

template <typename T, typename AABBTy> struct KDNode {
    KDNode(Type type, AABBTy bounds) : type(type), bounds(bounds) {}

    Type type;
    AABBTy bounds;
};

template <typename T, typename AABBTy>
using KDNodePtr = std::shared_ptr<KDNode<T, AABBTy> >;

template <typename T, typename AABBTy>
struct KDNodeInner : public KDNode<T, AABBTy> {
    KDNodeInner(AABBTy bounds, size_t dim, float split_val,
                KDNodePtr<T, AABBTy> left, KDNodePtr<T, AABBTy> right)
        : KDNode<T, AABBTy>(Type::INNER, bounds),
          dim(dim),
          split_val(split_val),
          left(left),
          right(right) {}

    size_t dim;
    float split_val;
    KDNodePtr<T, AABBTy> left, right;
};

template <typename T, typename AABBTy>
struct KDNodeLeaf : public KDNode<T, AABBTy> {
    KDNodeLeaf(AABBTy bounds, std::vector<T> objects)
        : KDNode<T, AABBTy>(Type::LEAF, bounds), objects(objects) {}
    std::vector<T> objects;
};

template <typename PolicyTy>
KDNodePtr<typename PolicyTy::value_type, typename PolicyTy::aabb_type> create(
    std::vector<typename PolicyTy::value_type> shapes,
    typename PolicyTy::aabb_type bounds, PolicyTy policy, size_t dim = 0,
    size_t depth = 0) {
    typedef typename PolicyTy::value_type T;
    typedef typename PolicyTy::aabb_type AABBTy;

    // Check if the terminal condition is true
    if (policy.terminal(shapes, bounds, depth))
        return std::make_shared<KDNodeLeaf<T, AABBTy> >(bounds, shapes);

    // Find a partitioning plane
    size_t split_dim;
    float split_val;
    std::tie(split_dim, split_val) = policy.partition(shapes, bounds, dim);

    // Create the two sub-AABBs
    AABBTy lhs_bound = bounds;
    lhs_bound.max[split_dim] = split_val;

    AABBTy rhs_bound = bounds;
    rhs_bound.min[split_dim] = split_val;

    // Separate our objects into left and right sides.
    std::vector<T> lhs, rhs;
    for (auto& shape : shapes) {
        auto& aabb = shape->get_aabb();
        if (aabb.max[split_dim] < split_val) {
            lhs.push_back(shape);
        } else if (aabb.min[split_dim] > split_val) {
            rhs.push_back(shape);
        } else {
            lhs.push_back(shape);
            rhs.push_back(shape);
        }
    }

    // Create the node recursively
    return std::make_shared<KDNodeInner<T, AABBTy> >(
        bounds, split_dim, split_val,
        create(lhs, lhs_bound, policy, split_dim, depth + 1),
        create(rhs, rhs_bound, policy, split_dim, depth + 1));
}

template <typename PolicyTy>
KDNodePtr<typename PolicyTy::value_type, typename PolicyTy::aabb_type> create(
    std::vector<typename PolicyTy::value_type> shapes, PolicyTy policy) {
    typedef typename PolicyTy::value_type T;
    typedef typename PolicyTy::aabb_type AABBTy;

    if (shapes.size() != 0) {
        AABBTy acc = shapes[0]->get_aabb();
        for (size_t i = 1; i < shapes.size(); ++i) {
            acc = acc + shapes[i]->get_aabb();
        }
        return create(shapes, acc, policy);
    }
    return std::make_shared<KDNodeLeaf<T, AABBTy> >(AABBTy(), shapes);
}

template <typename T, typename AABBTy, typename RayTy>
bool find_closest_hit(KDNodePtr<T, AABBTy>& node, RayTy& ray,
                      std::function<bool(std::vector<T>)> confirm_hit) {
    if(node->type == Type::INNER)
    {
        auto inner = std::static_pointer_cast<KDNodeInner<T, AABBTy>>(node);

        float left_t;
        bool left_hit = inner->right->bounds.test_hit(ray, left_t);

        float right_t;
        bool right_hit = inner->left->bounds.test_hit(ray, right_t);

        if(left_hit && right_hit)
        {
            if(left_t < right_t)
            {
                if(find_closest_hit(inner->left, ray, confirm_hit))
                    return true;
                if(find_closest_hit(inner->right, ray, confirm_hit))
                    return true;
            }
            else
            {
                if(find_closest_hit(inner->right, ray, confirm_hit))
                    return true;
                if(find_closest_hit(inner->left, ray, confirm_hit))
                    return true;
                
            }
        }
        else if(left_hit)
        {
            return find_closest_hit(inner->left, ray, confirm_hit);
        }
        else if(right_hit)
        {
            return find_closest_hit(inner->right, ray, confirm_hit);
        }
        return false;
    }
    else //Type::LEAF
    {
        // Simply determine if a hit appears in this objectlist. If it does, we're done.
        auto leaf = std::static_pointer_cast<KDNodeLeaf<T, AABBTy>>(node);
        return confirm_hit(leaf->objects);
    }
}


namespace policies {
template <typename T, typename AABBTy> struct CutInHalf {
    typedef T value_type;
    typedef AABBTy aabb_type;

    CutInHalf(size_t max_per_leaf, size_t max_depth)
        : max_per_leaf(max_per_leaf), max_depth(max_depth) {}

    bool terminal(std::vector<T>& shapes, AABBTy& bounds, size_t depth) {
        return depth > max_depth || shapes.size() < max_per_leaf;
    }

    std::tuple<size_t, float> partition(std::vector<T>& shapes, AABBTy& bounds,
                                        size_t dim) {
        // Determine the proper axis to use.
        ++dim;
        if (dim >= 3)
            dim = 0;

        // Now find the mid-point of the AABB in that dimension.
        float max = raytracer::val(bounds.max, dim),
              min = raytracer::val(bounds.min, dim);

        return std::tuple<size_t, float>(dim, (max + min) / 2.0f);
    }

    size_t max_per_leaf;
    size_t max_depth;
};
}
}
