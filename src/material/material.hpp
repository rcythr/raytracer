#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace raytracer {

struct Material {
    virtual ~Material() {}

    virtual std::string toString(size_t depth = 0) = 0;

    virtual glm::vec3 get_raw_color() = 0;

    virtual float ka() = 0;
    virtual float kd() = 0;
    virtual float ks() = 0;
    virtual float ke() = 0;
};

typedef std::shared_ptr<Material> MaterialPtr;
}
