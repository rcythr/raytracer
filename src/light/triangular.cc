#include "light/triangular.hpp"

#include "util/sample_triangle.hpp"
#include "util/sample_hemisphere.hpp"
#include "util/random_utils.hpp"

using namespace raytracer;

bool TriangularLight::gen_photon(Ray& result){
    // Calculate the normal vector
    glm::vec3 normal = glm::normalize(glm::cross(triangle->p1 - triangle->p0, triangle->p2 - triangle->p0));

    //Create random device for uniform distribution
    result.origin = sampleTriangle(triangle->p0, triangle->p1, triangle->p2, uniform_random(), uniform_random());
    result.direction = sampleHemisphereCosine(normal, uniform_random(), uniform_random());
    result.update();

    return true;
};
