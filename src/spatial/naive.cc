#include "spatial/naive.hpp"

#include "shape.hpp"

#include "util/string_mult.hpp"
#include <sstream>

using namespace raytracer;

void NaiveSpatialIndex::insert(ShapePtr ptr) 
{
    objects.push_back(ptr);
}

std::vector<ShapePtr> NaiveSpatialIndex::get_possible_hits(Ray& ray) 
{
    return objects;
}

std::string NaiveSpatialIndex::toString(size_t depth) 
{
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;

    ss << tabdepth << "TYPE: NAIVE\n";
    ss << tabdepth << "OBJECTS:\n";
    for(auto& obj : objects)
    {
        ss << obj->toString(depth+1);
    }

    return ss.str();
}
