#pragma once

#include <memory>

namespace raytracer {

struct Light {
    virtual ~Light() {}

    virtual std::string toString(size_t depth = 0) = 0;
};

typedef std::shared_ptr<Light> LightPtr;
}
