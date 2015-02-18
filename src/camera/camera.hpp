#pragma once

#include <memory>
#include <vector>
#include <functional>

#include <glm/glm.hpp>

#include "ray.hpp"

namespace raytracer {

struct SpatialIndex;
struct Light;

struct ViewPlane;

struct Camera {
    virtual ~Camera() {}

    virtual glm::mat4 build_transform_mat() = 0;

    virtual std::string toString(size_t depth = 0) = 0;

    virtual void spawn_rays(
        std::function<void(size_t, size_t, Ray&)> spawn_callback) = 0;

    std::shared_ptr<ViewPlane> view_plane;
};

typedef std::shared_ptr<Camera> CameraPtr;
}
