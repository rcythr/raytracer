
#include "shape/plane.hpp"

#include "util/string_mult.hpp"
#include "util/vec3_helpers.hpp"

#include <sstream>

using namespace raytracer;

void Plane::test_hit(Ray& ray, HitResult& result) {
    // First compute the dot product of the plane's normal and the ray's
    // direction.
    float rd_dot_pn = glm::dot(ray.direction, normal);

    // If 0 then no intersection exists at any point.
    // If < 0 then an intersection exists behind us, so ignore it.
    if (rd_dot_pn > 0) {
        // Now we find the distance along the ray where the intersection
        // lies
        // Set the value of the tmin because it's valid.
        float tval = glm::dot(point - ray.origin, normal) / rd_dot_pn;
        result.hit(shared_from_this(), tval, ray.origin + ray.direction * tval,
                   normal);
        return;
    }
    result.miss();
}

std::string Plane::toString(size_t depth) {
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;

    ss << tabdepth << "TYPE: PLANE\n";
    ss << tabdepth << "LOCATION: " << point << "\n";
    ss << tabdepth << "NORMAL: (" << normal << "\n";
    ss << tabdepth << "MATERIAL: \n";
    ss << material->toString(depth + 1);

    return ss.str();
}
