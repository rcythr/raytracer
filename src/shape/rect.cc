
#include "shape/rect.hpp"

#include "util/string_mult.hpp"
#include "util/vec3_helpers.hpp"

#include <iostream>
#include <sstream>

using namespace raytracer;

bool Rect::test_hit(Ray& ray, double& tmin)
{
    // First compute the dot product of the plane's normal and the ray's direction. 
    float rd_dot_pn = glm::dot(ray.direction, normal);

    //If 0 then no intersection exists at any point.
    if(rd_dot_pn > 0)
    {
        // Now we find the distance along the ray where the intersection lies
        float t = glm::dot(point - ray.origin, normal) / rd_dot_pn;
        
        std::cout << "HIT! t=" << t << std::endl;

        // Now calculate the intersection point
        glm::vec3 intersection = ray.origin + ray.direction * t;

        //TODO: We now have an intersection point, we need to find the x & y coordinates of this point on the plane.
        
        
        // Set the value of the tmin because it's valid.
        tmin = t;
        return true;
    }
    return false;
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
