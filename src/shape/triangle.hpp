#pragma once

#include "shape/shape.hpp"

#include <glm/glm.hpp>

namespace raytracer
{

struct Triangle : public Shape
{

    Triangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, std::shared_ptr<Material> mat) 
        : Shape(mat)
        , p0(p0)
        , p1(p1)
        , p2(p2)
    {}
    
    std::string toString(size_t depth=0) override;

    bool test_hit(Ray& ray, double& tmin) override;
    
    glm::vec3 p0, p1, p2;
};


}
