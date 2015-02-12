
#include "shape/rect.hpp"

#include "util/string_mult.hpp"
#include "util/vec3_helpers.hpp"
#include <sstream>

using namespace raytracer;

bool Rect::test_hit(Ray& ray, double& tmin)
{
}

std::string Rect::toString(size_t depth) 
{
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;

    ss << tabdepth << "TYPE: RECT\n";
    ss << tabdepth << "LOCATION: " << point << "\n";
    ss << tabdepth << "NORMAL: (" << normal << "\n";
    ss << tabdepth << "WIDTH: " << width << "\n";
    ss << tabdepth << "HEIGHT: " << height << "\n";
    ss << tabdepth << "MATERIAL: \n";
    ss << material->toString(depth+1);

    return ss.str();

}
