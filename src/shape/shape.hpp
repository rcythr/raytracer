#pragma once

#include <memory>

namespace raytracer {

struct Shape {

    virtual ~Shape() {}

    virtual std::string toString(size_t depth=0) = 0;

};

typedef std::shared_ptr<Shape> ShapePtr;
}
