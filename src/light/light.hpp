#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace raytracer {

enum class LightType { AMBIENT, DIRECTION, POINT };

struct Light {
    Light(glm::vec3 color) : color(color) {}

    virtual ~Light() {}

    virtual std::string toString(size_t depth = 0) = 0;

    virtual LightType get_type() = 0;

    glm::vec3 color;
};

typedef std::shared_ptr<Light> LightPtr;
}
