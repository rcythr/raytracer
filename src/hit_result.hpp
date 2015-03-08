#pragma once

namespace raytracer {

struct Shape;

struct HitResult {
    bool found_hit; // true iff a hit was detected.
    float tval;
    glm::vec3 intersection_point;
    glm::vec3 intersection_normal;
    std::shared_ptr<Shape> shape;

    HitResult() : found_hit(false) {}

    ~HitResult() = default;

    HitResult(const HitResult& other) = default;

    void miss() {
        this->found_hit = false;
        this->shape = nullptr;
    }

    void hit(std::shared_ptr<Shape> shape, float tval,
             glm::vec3 intersection_point, glm::vec3 intersection_normal) {
        this->found_hit = true;
        this->shape = shape;
        this->tval = tval;
        this->intersection_point = intersection_point;
        this->intersection_normal = intersection_normal;
    }
};
}
