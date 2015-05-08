#pragma once

namespace point_kdtree {

namespace detail {

template <typename PointTy, size_t K>
void findInRange(KDNode<PointTy>* node, PointTy& test, float range,
                 std::vector<PointTy>& result) {
    if (node == nullptr)
        return;

    size_t dim = node->dim;

    float distance = node->data[dim] - test[dim];

    if (std::abs(distance) <= range) {
        // Check both + Add Point
        result.push_back(node->data);
        findInRange<PointTy, K>(node->left, test, range, result);
        findInRange<PointTy, K>(node->right, test, range, result);
    } else if (distance > 0) {
        findInRange<PointTy, K>(node->left, test, range, result);
    } else if (distance < 0) {
        findInRange<PointTy, K>(node->right, test, range, result);
    }
}
}
}
