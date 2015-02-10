#include "shapes/sphere.hpp"

#include "util/vec3_helpers.hpp"
#include "util/string_mult.hpp"

#include <sstream>

using namespace raytracer;

std::string Sphere::toString(size_t depth)
{
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;

    ss << tabdepth << "TYPE: SPHERE\n";
    ss << tabdepth << "LOCATION: " << point << '\n';
    ss << tabdepth << "RADIUS: " << radius << '\n';
    ss << tabdepth << "MATERIAL: \n";
    ss << material->toString(depth+1);

    return ss.str();
}