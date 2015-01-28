#pragma once

#include "camera.hpp"

#include <glm/glm.hpp>

namespace raytracer
{

struct PinholeCamera : public Camera
{

    PinholeCamera( 
        int hres, 
        int vres,
        float pixel_size,
        int num_samples,
        glm::vec3 point, 
        glm::vec3 up, 
        glm::vec3 look_at, 
        float view_distance)
    {
    }

};

}
