#pragma once

#include <memory>
#include <vector>

namespace raytracer {

struct SpatialIndex;
struct Light;

struct ViewPlane;

struct Camera {

    virtual ~Camera() {}

    virtual void render(std::shared_ptr<SpatialIndex> index,
                        std::shared_ptr<Light> ambient_light,
                        std::vector<std::shared_ptr<Light>>& lights) = 0;

    virtual std::string toString(size_t depth=0) = 0;

    std::shared_ptr<ViewPlane> view_plane;
};

typedef std::shared_ptr<Camera> CameraPtr;
}
