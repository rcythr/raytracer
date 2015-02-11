#pragma once

#include <memory>

namespace raytracer {

struct ViewPlane;

struct Camera {

    virtual ~Camera() {}

    virtual std::string toString(size_t depth=0) = 0;

    std::shared_ptr<ViewPlane> view_plane;
};

typedef std::shared_ptr<Camera> CameraPtr;
}
