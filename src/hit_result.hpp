#pragma once

namespace raytracer
{

struct Shape;

struct HitResult
{
    bool found_hit; // true iff a hit was detected.
    float tval;
    glm::vec3 intersection_point;
    glm::vec3 intersection_normal;
    std::shared_ptr<Shape> shape;

    HitResult() = default;
    HitResult(const HitResult& other) = default;
    HitResult(HitResult&& other) = default;
    ~HitResult() = default;

    void miss() 
    { 
        this->found_hit = false;
    }

    void hit(std::shared_ptr<Shape> shape, float tval, glm::vec3 intersection_point, glm::vec3 intersection_normal)
    {
        this->found_hit = true;
        this->tval = tval;
        this->intersection_point = intersection_point;
        this->intersection_normal = intersection_normal;
    }

};

}
