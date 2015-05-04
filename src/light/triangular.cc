#include "light/triangular.hpp"

#include "util/sample_triangle.hpp"
#include "util/sample_hemisphere.hpp"

using namespace raytracer;

static thread_local std::random_device rd;
static thread_local std::mt19937 gen(rd());
static thread_local std::uniform_real_distribution<> dis(0, 1);

bool TriangularLight::gen_photon(Ray& result){
    // Calculate the normal vector
    glm::vec3 normal = glm::normalize(glm::cross(triangle->p1 - triangle->p0, triangle->p2 - triangle->p0));

    //Create random device for uniform distribution
    result.origin = sampleTriangle(triangle->p0, triangle->p1, triangle->p2, dis(gen), dis(gen));
    result.direction = sampleHemisphereCosine(normal, dis(gen), dis(gen));
    result.update();

    return true;
};
