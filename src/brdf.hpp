#pragma once

#include <memory>

namespace raytracer {

struct BRDF {};

typedef std::shared_ptr<BRDF> BRDFPtr;
}
