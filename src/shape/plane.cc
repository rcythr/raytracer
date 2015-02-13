
#include "shape/plane.hpp"

#include "util/string_mult.hpp"
#include "util/vec3_helpers.hpp"

#include <sstream>

using namespace raytracer;

bool Plane::test_hit(Ray& ray, double& tmin)
{
    // First compute the dot product of the plane's normal and the ray's direction. 
    float rd_dot_pn = glm::dot(ray.direction, normal);

    //If 0 then no intersection exists at any point.
    //If < 0 then an intersection exists behind us, so ignore it.
    if(rd_dot_pn > 0)
    {
        // Now we find the distance along the ray where the intersection lies
        // Set the value of the tmin because it's valid.
        tmin = glm::dot(point - ray.origin, normal) / rd_dot_pn;
        return true;
    }
    return false;
}

std::string Plane::toString(size_t depth) 
{
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;

    ss << tabdepth << "TYPE: RECT\n";
    ss << tabdepth << "LOCATION: " << point << "\n";
    ss << tabdepth << "NORMAL: (" << normal << "\n";
    ss << tabdepth << "MATERIAL: \n";
    ss << material->toString(depth+1);

    return ss.str();

}
