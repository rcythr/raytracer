#pragma once

#include <memory>

namespace raytracer {

struct Material {

    virtual ~Material() {}

    virtual std::string toString(size_t depth=0) = 0;

};

typedef std::shared_ptr<Material> MaterialPtr;
}
