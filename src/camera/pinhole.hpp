#pragma once

#include "camera/camera.hpp"

#include <glm/glm.hpp>

namespace raytracer {

struct PinholeCamera : public Camera {

    /**
     * Constructor for pinhole camera objects.
     * @param pixel_size the size of each output pixel in world coordinates.
     * @param num_samples the number of rays to fire into each pixel
     * @param point the location of the camera in world coordinates
     * @param up the direction of "up" for the camera in world coordinates
     * @param look_at the direction the camera is looking at (must be perpendicular to the up vector) in world coordinates.
     * @param view_distance the distance between the camera point and the viewplane.
     */
    PinholeCamera(float pixel_size, int num_samples, glm::vec3 point,
                  glm::vec3 up, glm::vec3 look_at, float view_distance)
        : num_samples(num_samples),
          pixel_size(pixel_size),
          view_distance(view_distance),
          point(point),
          up(up),
          look_at(look_at) {}

    ~PinholeCamera() override {}

    glm::mat4 build_transform_mat() override;

    void spawn_rays(
        std::function<void(size_t, size_t, Ray&)> spawn_callback) override;

    std::string toString(size_t depth) override;

private:
    int num_samples;
    float pixel_size, view_distance;
    glm::vec3 point, up, look_at;
};
}
