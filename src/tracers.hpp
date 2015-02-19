#pragma once

#include <memory>

namespace raytracer {

struct Kernel;
struct Camera;

void checkpoint1(Kernel* kernel, std::shared_ptr<Camera> camera);
}
