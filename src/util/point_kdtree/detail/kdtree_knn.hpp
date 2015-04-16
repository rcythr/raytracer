#pragma once

namespace point_kdtree {

namespace detail {

template <typename PointTy, size_t K> struct kNNConfig {

    kNNConfig(PointTy& test, size_t k) : test(test), k(k) {}

    typedef std::tuple<PointTy, float> kNNPair;

    struct kNNPairCompare {
        bool operator()(kNNPair& a, kNNPair& b) {
            return std::get<1>(a) < std::get<1>(b);
        }
    };

    PointTy test;
    size_t k;
    std::priority_queue<kNNPair, std::vector<kNNPair>, kNNPairCompare> bpq;

    bool bpq_full() { return bpq.size() == k; }

    void push_bpq(kNNPair& pair) {
        bpq.push(pair);

        if (bpq.size() > k) {
            bpq.pop();
        }
    }

    float worst_dist() { return std::get<1>(bpq.top()); }

    float distance(PointTy& val) {
        float d = 0;
        for (size_t i = 0; i < K; ++i) {
            d += pow(val[i] - test[i], 2);
        }
        return sqrt(d);
    }

    void consider(PointTy& val) {
        float ptDist = distance(val);
        kNNPair pair(val, ptDist);
        push_bpq(pair);
    }
};

template <typename PointTy, size_t K>
void kNearestNeighborRec(kNNConfig<PointTy, K>& cfg, KDNode<PointTy>* node,
                         size_t dim = 0) {
    if (node == nullptr)
        return;

    if (dim == K)
        dim = 0;

    cfg.consider(node->data);

    if (cfg.test[dim] < node->data[dim]) {
        kNearestNeighborRec(cfg, node->left, dim + 1);
        if (!cfg.bpq_full() ||
            abs(node->data[dim] - cfg.test[dim]) < cfg.worst_dist()) {
            kNearestNeighborRec(cfg, node->right, dim + 1);
        }
    } else {
        kNearestNeighborRec(cfg, node->right, dim + 1);
        if (!cfg.bpq_full() ||
            abs(node->data[dim] - cfg.test[dim]) < cfg.worst_dist()) {
            kNearestNeighborRec(cfg, node->left, dim + 1);
        }
    }
}

template <typename PointTy, size_t K>
void kNearestNeighbors(KDNode<PointTy>* root, PointTy test, size_t k,
                       std::vector<PointTy>& results) {
    kNNConfig<PointTy, K> cfg(test, k);
    kNearestNeighborRec<PointTy, K>(cfg, root);

    while (!cfg.bpq.empty()) {
        results.push_back(std::get<0>(cfg.bpq.top()));
        cfg.bpq.pop();
    }
}
}
}
