#pragma once

#include <algorithm>
#include <functional>
#include <queue>
#include <vector>

#include "detail/kdtree_impl.hpp"
#include "detail/kdtree_construct.hpp"
#include "detail/kdtree_find.hpp"
#include "detail/kdtree_iterate.hpp"
#include "detail/kdtree_nn.hpp"
#include "detail/kdtree_knn.hpp"

namespace point_kdtree {

template <typename PointTy, size_t K>
struct PointKDTree {
    KDNode<PointTy> *root;

    PointKDTree(std::vector<PointTy> &lst)
        : root(detail::construct<PointTy, K>(lst, 0)) {}

    ~PointKDTree() {
        if (root) delete root;
    }

    bool nearestNeighbor(PointTy p, PointTy& result) 
    {
        return detail::nearestNeighbor<PointTy, K>(root, p, result);
    }

    std::vector<PointTy> kNearestNeighbors(PointTy p, size_t k) {
        std::vector<PointTy> result;
        detail::kNearestNeighbors<PointTy, K>(root, p, k, result);
        return result;
    }

    std::vector<PointTy> findAllInRange(PointTy pt, float range) {
       std::vector<PointTy> result;
       detail::findInRange<PointTy, K>(root, pt, range, result);
       return result;
    }

    void iterate(IterationOrder order, std::function<void(PointTy &, size_t)> func) {
        detail::iterate<PointTy, K>(root, func, order, 0);
    }
};
}
