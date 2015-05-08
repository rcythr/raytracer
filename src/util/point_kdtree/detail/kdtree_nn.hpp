#pragma once

namespace point_kdtree {
namespace detail {

template <typename PointTy, size_t K> struct NNConfig {
    NNConfig(PointTy& test) : test(test), has_guess(false) {}

    PointTy& test;

    bool has_guess;
    PointTy guess;
    float bestDist;

    float distance(PointTy& val) {
        float d = 0;
        for (size_t i = 0; i < K; ++i) {
            d += pow(val[i] - test[i], 2);
        }
        return sqrt(d);
    }

    void consider(PointTy& val) {
        float ptDist = distance(val);
        if (!has_guess || ptDist < bestDist) {
            guess = val;
            bestDist = ptDist;
            has_guess = true;
        }
    }
};

template <typename PointTy, size_t K>
void nearestNeighborRec(NNConfig<PointTy, K>& cfg, KDNode<PointTy>* node) {
    if (node == nullptr)
        return;

    size_t dim = node->dim;

    // Consider the current point
    cfg.consider(node->data);

    // Now check the children, as needed.
    if (cfg.test[dim] < node->data[dim]) {
        nearestNeighborRec(cfg, node->left);
        if (std::abs(node->data[dim] - cfg.test[dim]) < cfg.bestDist) {
            nearestNeighborRec(cfg, node->right);
        }
    } else {
        nearestNeighborRec(cfg, node->right);
        if (std::abs(node->data[dim] - cfg.test[dim]) < cfg.bestDist) {
            nearestNeighborRec(cfg, node->left);
        }
    }
}

template <typename PointTy, size_t K>
bool nearestNeighbor(KDNode<PointTy>* node, PointTy test, PointTy& result) {
    NNConfig<PointTy, K> cfg(test);
    nearestNeighborRec(cfg, node);
    if (cfg.has_guess) {
        result = cfg.guess;
        return true;
    } else {
        return false;
    }
}
}
}
