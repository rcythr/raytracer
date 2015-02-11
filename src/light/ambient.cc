
#include "light/ambient.hpp"

#include <sstream>
#include "util/string_mult.hpp"

using namespace raytracer;

std::string AmbientLight::toString(size_t depth)
{
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;
    ss << tabdepth << "TYPE: AMBIENT\n";
    ss << tabdepth << "SCALE RADIANCE: " << scale_radiance << "\n";

    return ss.str();
}
