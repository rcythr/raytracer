#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "ray.hpp"

namespace raytracer {

enum class LightType { 
    AMBIENT, 
    DIRECTION, 
    POINT,
    TRIANGULAR,
    SPHERICAL };

struct Light {
    Light(float intensity, glm::vec3 color) : intensity(intensity), color(color) {}

    virtual ~Light() {}

    virtual LightType get_type() = 0;

    virtual bool gen_photon(Ray& r) { return false; }

    float intensity;
    glm::vec3 color;
};

typedef std::shared_ptr<Light> LightPtr;
}
