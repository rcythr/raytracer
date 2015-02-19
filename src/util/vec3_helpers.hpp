#pragma once

#include <glm/glm.hpp>
#include <ostream>

namespace raytracer {

std::ostream& operator<<(std::ostream& os, glm::vec3 vector);
std::ostream& operator<<(std::ostream& os, glm::vec4 vector);

}
