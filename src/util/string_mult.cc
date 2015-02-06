
#include "util/string_mult.hpp"

using namespace raytracer;

std::string raytracer::operator*(std::string&& s, size_t num) {
    std::string result;
    for(size_t i=0; i < num; ++i)
    {
        result += s;
    }
    return result;
}
