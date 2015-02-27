
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

float raytracer::val(glm::vec3& obj, size_t indx) {
    switch (indx) {
        case 0:
            return obj.x;
        case 1:
            return obj.y;
        case 2:
            return obj.z;
        default:
            throw std::runtime_error("Index out of bounds for vec3.");
    }
}

float raytracer::val(glm::vec4& obj, size_t indx) {
    switch (indx) {
        case 0:
            return obj.x;
        case 1:
            return obj.y;
        case 2:
            return obj.z;
        case 3:
            return obj.w;
        default:
            throw std::runtime_error("Index out of bounds for vec3.");
    }
}
