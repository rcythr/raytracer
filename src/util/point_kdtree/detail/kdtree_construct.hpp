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
        else if (pivot[dim] < pt[dim])
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
KDNode<PointTy> *construct(std::vector<PointTy> lst) {
    auto result = new KDNode<PointTy>();

    // Base case: If we have exactly 1 item we're done.
    if (lst.size() == 1) {
        result->dim = 0;
        result->data = lst[0];
    } else {
        glm::vec3 max = glm::vec3(lst[0][0], lst[0][1], lst[0][2]);
        glm::vec3 min = max;

        for(auto& point : lst) {
            if(min.x > point[0]) min.x = point[0];
            if(min.y > point[1]) min.y = point[1];
            if(min.z > point[2]) min.z = point[2];
            if(max.x < point[0]) max.x = point[0];
            if(max.y < point[1]) max.y = point[1];
            if(max.z < point[2]) max.z = point[2];
        }

        auto diff = glm::vec3(std::abs(max.x - min.x),
                              std::abs(max.y - min.y),
                              std::abs(max.z - min.z));

        size_t dim = 2; // Assume z
        if(diff.x > diff.y) {
            if(diff.x > diff.z)
                dim = 0;
        } else {
            if(diff.y > diff.z)
                dim = 1;
        }

        auto median = detail::findMedian<PointTy>(lst, dim);

        std::vector<PointTy> left, right;
        for (auto &ele : lst) {
            if (ele == median)
                continue;

            if (ele[dim] < median[dim]) {
                left.push_back(ele);
            } else {
                right.push_back(ele);
            }
        }

        // Recursive case:
        result->data = median;
        result->dim = dim;

        if (left.size() != 0)
            result->left = construct<PointTy, K>(left);

        if (right.size() != 0)
            result->right = construct<PointTy, K>(right);
    }
    return result;
}
}
}
