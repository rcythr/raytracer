#pragma once

namespace kdtree
{

struct KDNode
{
};

typedef std::shared_ptr<kdtree::KDNode> KDNodePtr;

template<typename T>
KDNodePtr create(std::vector<T> shapes)
{

}

template<typename T, typename RayTy>
void find_closest_hit(KDNodePtr node, RayTy ray, std::function<bool(std::vector<T>)> confirm_hit)
{
}

}
