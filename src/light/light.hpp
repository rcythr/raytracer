#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace raytracer {

enum class LightType { AMBIENT, DIRECTION, POINT };

struct Light {
    Light(float intensity, glm::vec3 color) : intensity(intensity), color(color) {}

    virtual ~Light() {}

    virtual std::string toString(size_t depth = 0) = 0;

    virtual LightType get_type() = 0;

    float intensity;
    glm::vec3 color;
};

typedef std::shared_ptr<Light> LightPtr;
}
