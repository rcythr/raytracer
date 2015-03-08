#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace raytracer {

struct Light {
    virtual ~Light() {}

    virtual std::string toString(size_t depth = 0) = 0;

    glm::vec3 color;
};

typedef std::shared_ptr<Light> LightPtr;
}
