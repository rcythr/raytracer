#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "ray.hpp"

namespace raytracer {

struct SpatialIndex;
struct Light;

struct ViewPlane;

struct RayContext {
    size_t row, col;
    Ray ray;
};

struct Camera {

    /**
     * Virtual destructor
     */
    virtual ~Camera() {}

    /**
     * Constructs a matrix to transform points and vectors from world space into
     * camera space.
     */
    virtual glm::mat4 build_transform_mat() = 0;

    /**
     * Spawns rays according to the camera's location and calls the given
     * function
     *   for each ray with (row, col, ray) information.
     * @param spawn_callback the function to call for each ray with row, col,
     * and the ray.
     */
    virtual void spawn_rays(std::function<void(size_t, size_t, Ray&)> spawn_callback) = 0;

    virtual size_t get_num_samples() = 0;

    /**
     * Returns the number of bounces that should be used before stopping.
     */
    virtual size_t get_num_bounces() = 0;

    /**
     * The view plane object that this camera should render to.
     */
    std::shared_ptr<ViewPlane> view_plane;
};

typedef std::shared_ptr<Camera> CameraPtr;
}
