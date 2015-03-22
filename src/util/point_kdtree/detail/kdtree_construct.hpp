#pragma once

namespace point_kdtree {

namespace detail {

template <typename PointTy>
PointTy select(std::vector<PointTy> &lst, size_t dim, size_t indx) {
    PointTy pivot = lst[rand() % lst.size()];

    std::vector<PointTy> smaller, larger;
    for (auto &pt : lst) {
        if (pt[dim] < pivot[dim])
            smaller.push_back(pt);
        else if(pivot[dim] < pt[dim])
            larger.push_back(pt);
    }

    size_t a = lst.size() - larger.size();
    if (indx < smaller.size())
        return select<PointTy>(smaller, dim, indx);
    else if (indx > a)
        return select<PointTy>(larger, dim, indx - a);
    else
        return pivot;
}

template <typename PointTy>
PointTy findMedian(std::vector<PointTy> &lst, size_t dim) {
    return select(lst, dim, lst.size() / 2);
}

template <typename PointTy, size_t K>
KDNode<PointTy>* construct(std::vector<PointTy> lst, size_t dim = 0) {
    auto result = new KDNode<PointTy>();

    // Base case: If we have exactly 1 item we're done.
    if (lst.size() == 1) {
        result->data = lst[0];
    } else {
        // Adjust the dim. This avoids modulo division.
        if (dim == K) dim = 0;

        auto median = detail::findMedian<PointTy>(lst, dim);

        std::vector<PointTy> left, right;
        for (auto &ele : lst) {
            if(ele == median)
                continue;

            if (ele[dim] < median[dim]) {
                left.push_back(ele);
            } else {
                right.push_back(ele);
            }
        }

        // Recursive case:
        result->data = median;

        if (left.size() != 0)
            result->left = construct<PointTy, K>(left, dim + 1);

        if (right.size() != 0)
            result->right = construct<PointTy, K>(right, dim + 1);
    }
    return result;
}

}
}
