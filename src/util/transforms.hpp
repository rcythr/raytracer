#pragma once

#include <glm/glm.hpp>

namespace raytracer
{

glm::mat4 buildTranslation(glm::vec3 trans);

glm::mat4 buildRotationX(float theta);

glm::mat4 buildRotationY(float theta);

glm::mat4 buildRotationZ(float theta);

glm::mat4 buildScale(float scale);

}
