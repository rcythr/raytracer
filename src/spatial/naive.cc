#include "spatial/naive.hpp"

#include "util/string_mult.hpp"
#include <sstream>

using namespace raytracer;

void NaiveSpatialIndex::insert(ShapePtr ptr) 
{
    objects.push_back(ptr);
}

void NaiveSpatialIndex::find_closest_hit(Ray& ray, std::function<void(ShapePtr&, double)> hit_callback)
{
    double tmin, t;
    ShapePtr hit_obj = nullptr;
    for(auto obj : objects)
    {
        if(obj->test_hit(ray, t))
        {
            if(hit_obj == nullptr || t < tmin)
            {
                tmin = t;
                hit_obj = obj;
            }
        }
    }

    if(hit_obj != nullptr)
    {
        hit_callback(hit_obj, tmin);
    }
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
