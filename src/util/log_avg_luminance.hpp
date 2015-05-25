#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace raytracer
{

struct ViewPlane;

float luminance(const glm::vec3 color);

float avg_luminance(std::shared_ptr<ViewPlane>& vp, float l_max); 
float log_avg_luminance(std::shared_ptr<ViewPlane>& vp, float l_max); 

}
