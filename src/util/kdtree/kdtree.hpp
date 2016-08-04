#pragma once

#include <tuple>
#include <algorithm>
#include <iostream>
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
KDNodePtr<typename PolicyTy::value_type, typename PolicyTy::aabb_type>
create_rec(std::vector<typename PolicyTy::value_type> shapes,
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
    AABBTy lhs_bound;
    lhs_bound.max = bounds.max;
    lhs_bound.min = bounds.min;
    lhs_bound.max[split_dim] = split_val;

    AABBTy rhs_bound;
    rhs_bound.max = bounds.max;
    rhs_bound.min = bounds.min;
    rhs_bound.min[split_dim] = split_val;

    // Separate our objects into left and right sides.
    std::vector<T> lhs, rhs;
    for (auto& shape : shapes) {
        if (shape->test_hit(lhs_bound)) {
            lhs.push_back(shape);
        }
        if (shape->test_hit(rhs_bound)) {
            rhs.push_back(shape);
        }
    }

    auto left = create_rec(lhs, lhs_bound, policy, split_dim, depth + 1);
    auto right = create_rec(rhs, rhs_bound, policy, split_dim, depth + 1);

    // Create the node recursively
    return std::make_shared<KDNodeInner<T, AABBTy> >(bounds, split_dim,
                                                     split_val, left, right);
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

        // Adjust the bounding box to contain a little extra space than the
        // actual object extents.
        acc.min.x -= 2.5f;
        acc.min.y -= 2.5f;
        acc.min.z -= 2.5f;
        acc.max.x += 2.5f;
        acc.max.y += 2.5f;
        acc.max.z += 2.5f;

        return create_rec(shapes, acc, policy);
    }
    return std::make_shared<KDNodeLeaf<T, AABBTy> >(AABBTy(), shapes);
}

template <typename T, typename AABBTy, typename RayTy>
bool find_closest_hit(KDNodePtr<T, AABBTy>& node, const RayTy& ray,
                      std::function<bool(AABBTy&,std::vector<T>&)> confirm_hit) {
    if (node->type == Type::INNER) {
        auto inner = std::static_pointer_cast<KDNodeInner<T, AABBTy> >(node);

        float left_t;
        bool left_hit = inner->left->bounds.test_hit(ray, left_t);

        float right_t;
        bool right_hit = inner->right->bounds.test_hit(ray, right_t);

        if (left_hit && right_hit) {
            if (left_t < right_t) {
                if (find_closest_hit(inner->left, ray, confirm_hit))
                    return true;
                if (find_closest_hit(inner->right, ray, confirm_hit))
                    return true;
            } else {
                if (find_closest_hit(inner->right, ray, confirm_hit))
                    return true;
                if (find_closest_hit(inner->left, ray, confirm_hit))
                    return true;
            }
        } else if (left_hit) {
            return find_closest_hit(inner->left, ray, confirm_hit);
        } else if (right_hit) {
            return find_closest_hit(inner->right, ray, confirm_hit);
        }
        return false;
    } else // Type::LEAF
    {
        // Simply determine if a hit appears in this objectlist. If it does,
        // we're done.
        auto leaf = std::static_pointer_cast<KDNodeLeaf<T, AABBTy> >(node);
        return confirm_hit(leaf->bounds, leaf->objects);
    }
}

template <typename T, typename AABBTy>
void dump(KDNodePtr<T, AABBTy>& node,
          std::function<void(size_t, AABBTy&, size_t, float)> dump_fun,
          size_t depth = 0) {
    if (node->type == Type::INNER) {
        auto inner = std::static_pointer_cast<KDNodeInner<T, AABBTy> >(node);
        dump_fun(depth, inner->bounds, inner->dim, inner->split_val);
        dump(inner->left, dump_fun, depth + 1);
        dump(inner->right, dump_fun, depth + 1);
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
        float max = bounds.max[dim], min = bounds.min[dim];

        return std::tuple<size_t, float>(dim, (max + min) / 2.0f);
    }

    size_t max_per_leaf;
    size_t max_depth;
};

template <typename T, typename AABBTy> struct BestSplit {
    enum class TokenType { Open, Close };

    struct Token {
        TokenType type;
        size_t object_indx;
        float cost;
        float value;

        float get_cost() const {
            return (type == TokenType::Open) ? cost : -cost;
        }

        bool operator<(const Token& other) const { return value < other.value; }
    };

    typedef T value_type;
    typedef AABBTy aabb_type;

    BestSplit(size_t max_per_leaf, size_t max_depth)
        : max_per_leaf(max_per_leaf), max_depth(max_depth) {}

    bool terminal(std::vector<T>& shapes, AABBTy& bounds, size_t depth) {
        return depth > max_depth || shapes.size() < max_per_leaf;
    }

    std::tuple<size_t, float> partition(std::vector<T>& shapes, AABBTy& bounds,
                                        size_t dim) {
        // First we need to break the shapes into start and end 'tokens' for
        // each dim.
        std::vector<Token> x_tokens, y_tokens, z_tokens;

        size_t num_shapes = shapes.size();
        for (size_t i = 0; i < num_shapes; ++i) {
            auto& aabb = shapes[i]->get_aabb();
            x_tokens.push_back(Token{ TokenType::Open, i, 1.0f, aabb.min.x });
            x_tokens.push_back(Token{ TokenType::Close, i, 1.0f, aabb.max.x });
            y_tokens.push_back(Token{ TokenType::Open, i, 1.0f, aabb.min.y });
            y_tokens.push_back(Token{ TokenType::Close, i, 1.0f, aabb.max.y });
            z_tokens.push_back(Token{ TokenType::Open, i, 1.0f, aabb.min.z });
            z_tokens.push_back(Token{ TokenType::Close, i, 1.0f, aabb.max.z });
        }

        // Now we want to find the best candidate for each dim.
        float best_x_loc, x_abs_diff;
        std::tie(best_x_loc, x_abs_diff) = find_best_candidate(x_tokens);

        float best_y_loc, y_abs_diff;
        std::tie(best_y_loc, y_abs_diff) = find_best_candidate(y_tokens);

        float best_z_loc, z_abs_diff;
        std::tie(best_z_loc, z_abs_diff) = find_best_candidate(z_tokens);

        // Now we have to compare the 3 abs_diffs to find the smallest.
        if (x_abs_diff <= y_abs_diff) {
            if (x_abs_diff <= z_abs_diff) {
                return std::tuple<size_t, float>(0, best_x_loc);
            }
        } else {
            if (y_abs_diff <= z_abs_diff) {
                return std::tuple<size_t, float>(1, best_y_loc);
            }
        }
        return std::tuple<size_t, float>(2, best_z_loc);
    }

  private:
    size_t max_per_leaf;
    size_t max_depth;

    std::tuple<float, float> find_best_candidate(std::vector<Token>& tokens) {
        bool found_best = false;
        float best_location, best_abs_cost;

        size_t num_tokens = tokens.size();

        // First sort the tokens.
        std::sort(tokens.begin(), tokens.end());

        // Calculate our prefix and postfix sums of cost.
        std::vector<float> prefix_cost(num_tokens + 1);
        prefix_cost[0] = 0.0f;
        for (size_t i = 0; i < num_tokens; ++i)
            prefix_cost[i + 1] = prefix_cost[i] + tokens[i].get_cost();

        std::vector<float> postfix_cost(num_tokens + 1);
        postfix_cost[num_tokens] = 0.0f;
        for (size_t i = num_tokens; i > 0; --i)
            postfix_cost[i - 1] = postfix_cost[i] + tokens[i - 1].get_cost();

        // If token's size is N then there are N+1 **distinct** partition
        // planes.
        for (size_t i = 0; i < num_tokens + 1; ++i) {
            float location, abs_cost;
            if (i == 0) // First Plane
            {
                location =
                    tokens[i].value -
                    0.0001f; // Just a little bit left of the first object.
            } else if (i == num_tokens - 1) // Last Plane
            {
                location =
                    tokens[i].value +
                    0.0001f; // Just a little bit right of the first object.
            } else           // General Case
            {
                location = (tokens[i].value + tokens[i - 1].value) / 2.0f;
            }

            // Calculate the cost difference between left and right sides.
            abs_cost = std::abs(prefix_cost[i] - postfix_cost[i]);

            // We want the lowest difference.
            if (!found_best || abs_cost < best_abs_cost) {
                best_location = location;
                best_abs_cost = abs_cost;
                found_best = true;
            }
        }

        // Return what we found.
        return std::tuple<float, float>(best_location, best_abs_cost);
    }
};
}
}
