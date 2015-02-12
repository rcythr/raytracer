#pragma once

#include <memory>

#include "ray.hpp"

namespace raytracer {

struct Material;

struct Shape {

    Shape(std::shared_ptr<Material> material)
        : material(material)
    {}

    virtual ~Shape() {}

    virtual std::string toString(size_t depth=0) = 0;

    virtual bool test_hit(Ray& ray, double& tmin) = 0;
    
    std::shared_ptr<Material> material;
};

typedef std::shared_ptr<Shape> ShapePtr;
}
