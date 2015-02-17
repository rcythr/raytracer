
#include "shape/triangle.hpp"
#include "material/material.hpp"

#include <sstream>
#include "util/string_mult.hpp"
#include "util/vec3_helpers.hpp"
#include "util/triangle_test.hpp"

using namespace raytracer;

std::string Triangle::toString(size_t depth)
{
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;
    ss << tabdepth << "TYPE: TRIANGLE" << std::endl;
    ss << tabdepth << "P_0: " << p0 << std::endl;
    ss << tabdepth << "P_1: " << p1 << std::endl;
    ss << tabdepth << "P_2: " << p2 << std::endl;
    ss << tabdepth << "MATERIAL: " << std::endl;
    ss << material->toString(depth+1);
    return ss.str();
}

bool Triangle::test_hit(Ray& ray, double& tmin)
{
    return test_triangle_hit(ray, p0, p1, p2, tmin);
}
    
