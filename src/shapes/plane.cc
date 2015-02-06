#include "shapes/plane.hpp"

#include "util/vec3_helpers.hpp"
#include "util/string_mult.hpp"

#include <sstream>

using namespace raytracer;

std::string Plane::toString(size_t depth) 
{
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;

    ss << tabdepth << "TYPE: PLANE\n";
    ss << tabdepth << "LOCATION: " << point << "\n";
    ss << tabdepth << "NORMAL: (" << normal << "\n";
    ss << tabdepth << "MATERIAL: \n";
    ss << material->toString(depth+1);

    return ss.str();
}
