#pragma once

#include <glm/glm.hpp>

glm::vec3 findPerpendicular(glm::vec3 vec);

glm::vec3 sampleHemisphereCosine(glm::vec3& normal, float Xi1, float Xi2);

glm::vec3 sampleHemisphereUniform(glm::vec3& normal, float Xi1, float Xi2);
