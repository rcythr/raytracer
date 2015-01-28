#pragma once

#include "material.hpp"
#include "color.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct Matte : public Material {

    Matte(float ka, float kd, ColorPtr color) {}
};
}
