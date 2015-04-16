#pragma once

namespace point_kdtree {

enum class IterationOrder { PRE_ORDER, IN_ORDER, POST_ORDER };

namespace detail {

template <typename PointTy, size_t K>
void iterate(KDNode<PointTy> *node, std::function<void(PointTy &, size_t)> func,
             IterationOrder order, int depth = 0) {
    if (node == nullptr)
        return;

    switch (order) {
        case IterationOrder::PRE_ORDER:
            func(node->data, depth);
            iterate<PointTy, K>(node->left, func, order, depth + 1);
            iterate<PointTy, K>(node->right, func, order, depth + 1);
            break;
        case IterationOrder::IN_ORDER:
            iterate<PointTy, K>(node->left, func, order, depth + 1);
            func(node->data, depth);
            iterate<PointTy, K>(node->right, func, order, depth + 1);
            break;
        case IterationOrder::POST_ORDER:
            iterate<PointTy, K>(node->left, func, order, depth + 1);
            iterate<PointTy, K>(node->right, func, order, depth + 1);
            func(node->data, depth);
            break;
    }
}
}
}
