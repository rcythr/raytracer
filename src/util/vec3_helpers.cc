
#include "util/vec3_helpers.hpp"

using namespace raytracer;

std::ostream& raytracer::operator<<(std::ostream& os, glm::vec3 vector) {
    os << '(' << vector.x << ',' << vector.y << ',' << vector.z << ')';
    return os;
}

std::ostream& raytracer::operator<<(std::ostream& os, glm::vec4 vector) {
    os << '(' << vector.x << ',' << vector.y << ',' << vector.z << ','
       << vector.w << ')';
    return os;
}

