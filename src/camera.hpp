#pragma once

#include <memory>

namespace raytracer {

struct Camera {

    virtual ~Camera() {}

    virtual std::string toString(size_t depth=0) = 0;

};

typedef std::shared_ptr<Camera> CameraPtr;
}
