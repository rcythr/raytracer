#pragma once

namespace point_kdtree {

template <typename PointTy>
struct KDNode {
    typedef PointTy point_type;

    PointTy data;
    KDNode<PointTy> *left, *right;

    KDNode() : left(nullptr), right(nullptr) {}

    ~KDNode() {
        if (left != nullptr) delete left;
        if (right != nullptr) delete right;
    }
};

}
