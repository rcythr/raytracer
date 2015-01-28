#pragma once

#include <memory>

namespace raytracer {

struct Color {

    Color(glm::vec3 values) {}
};

typedef std::shared_ptr<Color> ColorPtr;
}
