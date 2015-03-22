#pragma once

namespace point_kdtree {

namespace detail {

template<typename PointTy, size_t K>
void findInRange(KDNode<PointTy> *node, PointTy test, float range, std::vector<std::tuple<PointTy, float>> & result, size_t dim=0) {
    if(node == nullptr)
        return;

    if(dim == K) 
        dim = 0;

    float distance = node->data[dim] - test[dim];

    if(abs(distance) <= range) {
        // Check both + Add Point
        result.push_back(std::tuple<PointTy, float>(node->data, distance));
        findInRange(node->left, test, range, result, dim + 1);
        findInRange(node->right, test, range, result, dim + 1);
    } else if(distance > 0) {
        findInRange(node->left, test, range, result, dim + 1);
    } else if(distance < 0) {
        findInRange(node->right, test, range, result, dim + 1);
    }
}

}
}
