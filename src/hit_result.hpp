#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "ray.hpp"

namespace raytracer {

struct Shape;

struct HitResult {
    bool found_hit; // true iff a hit was detected.
    float tval, u, v;
    glm::vec3 intersection_point;
    glm::vec3 intersection_normal;
    std::shared_ptr<Shape> shape;
    Ray incoming_ray;

    HitResult() : found_hit(false) {}

    ~HitResult() = default;

    HitResult(const HitResult& other) = default;

    void miss() {
        this->found_hit = false;
        this->shape = nullptr;
    }

    void hit(std::shared_ptr<Shape> shape, float tval,
             float u, float v,
             glm::vec3 intersection_point, glm::vec3 intersection_normal, 
             Ray incoming_ray) {
        this->found_hit = true;
        this->shape = shape;
        this->tval = tval;
        this->u = u;
        this->v = v;
        this->intersection_point = intersection_point;
        this->intersection_normal = intersection_normal;
        this->incoming_ray = incoming_ray;
    }
};
}
