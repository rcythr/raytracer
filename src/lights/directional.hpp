#pragma once

#include "light.hpp"

namespace raytracer {

struct DirectionalLight : public Light {

    DirectionalLight(glm::vec3 direction, float scale_radiance) {}
};
}
