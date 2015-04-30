#include "light/triangular.hpp"

#include "util/sample_triangle.hpp"
#include "util/sample_hemisphere.hpp"

using namespace raytracer;

std::vector<Ray> TriangularLight::get_photons(int n){
    //Create Ray vector
    std::vector<Ray> rays;

    // Calculate the normal vector
    glm::vec3 normal = glm::normalize(glm::cross(triangle->p1 - triangle->p0, triangle->p2 - triangle->p0));

    //Create random device for uniform distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    for(int i = 0; i < n; i++){
        Ray r;
        r.origin = sampleTriangle(triangle->p0, triangle->p1, triangle->p2, dis(gen), dis(gen));
        r.direction = sampleHemisphere(normal, dis(gen), dis(gen));
        r.update();
        rays.push_back(r);
    }
    return rays;
};
