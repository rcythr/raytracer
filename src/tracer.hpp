#pragma once

#include <memory>

namespace raytracer {

struct Tracer {};

typedef std::shared_ptr<Tracer> TracerPtr;
}
