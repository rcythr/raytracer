
#include "light/directional.hpp"

#include <sstream>
#include "util/string_mult.hpp"
#include "util/vec3_helpers.hpp"

using namespace raytracer;

std::string DirectionalLight::toString(size_t depth) 
{
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;
    ss << tabdepth << "TYPE: DIRECTIONAL\n";
    ss << tabdepth << "DIRECTION: " << direction << "\n";
    ss << tabdepth << "SCALE RADIANCE: " << scale_radiance << "\n";

    return ss.str();
}
