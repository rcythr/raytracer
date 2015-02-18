#pragma once

#include <memory>
#include <string>

namespace raytracer {

struct BRDF {
    virtual ~BRDF() {}

    virtual std::string toString(size_t depth = 0) = 0;
};

typedef std::shared_ptr<BRDF> BRDFPtr;
}
