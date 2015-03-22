#pragma once

namespace point_kdtree {

namespace detail {

template<typename PointTy, size_t K>
void findInRange(KDNode<PointTy> *node, PointTy& test, float range, std::vector<PointTy> & result, size_t dim=0) {
    if(node == nullptr)
        return;

    if(dim == K) 
        dim = 0;

    float distance = node->data[dim] - test[dim];

    if(abs(distance) <= range) {
        // Check both + Add Point
        result.push_back(node->data);
        findInRange<PointTy, K>(node->left, test, range, result, dim + 1);
        findInRange<PointTy, K>(node->right, test, range, result, dim + 1);
    } else if(distance > 0) {
        findInRange<PointTy, K>(node->left, test, range, result, dim + 1);
    } else if(distance < 0) {
        findInRange<PointTy, K>(node->right, test, range, result, dim + 1);
    }
}

}
}
