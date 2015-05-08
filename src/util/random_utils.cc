#include "util/random_utils.hpp"

#include <random>

static thread_local std::random_device rd;
static thread_local std::mt19937 gen(rd());
static thread_local std::uniform_real_distribution<> dis(0, 1);

float raytracer::uniform_random() { return dis(gen); }
