#include "light/point.hpp"

#include "util/string_mult.hpp"
#include "util/vec3_helpers.hpp"
#include <sstream>

using namespace raytracer;

std::string PointLight::toString(size_t depth) {
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;

    ss << tabdepth << "TYPE: POINT\n";
    ss << tabdepth << "POINT: " << point << "\n";
    ss << tabdepth << "COLOR: (" << color.r << ',' << color.g << ',' << color.b
       << ")\n";

    return ss.str();
}
