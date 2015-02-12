
#include "material/matte.hpp"

#include <sstream>
#include "util/string_mult.hpp"

using namespace raytracer;

glm::vec3 Matte::get_raw_color()
{
    return color;
}

std::string Matte::toString(size_t depth)
{
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;
    ss << tabdepth << "TYPE: MATTE\n";
    ss << tabdepth << "KA: " << ka << "\n";
    ss << tabdepth << "KD: " << kd << "\n";
    ss << tabdepth << "COLOR: (" << color.r << ',' << color.g << ',' << color.b << ")\n";
    
    return ss.str();
}
