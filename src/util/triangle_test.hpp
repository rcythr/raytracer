#pragma once

#include "ray.hpp"
#include <glm/glm.hpp>

namespace raytracer
{

bool test_triangle_hit(Ray& ray, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, double& min);

}
