#include "light/triangular.hpp"

using namespace raytracer;

std::vector<Ray> TriangularLight::get_photons(int n){
    //Create Ray vector
    std::vector<Ray> rays;

    //Create random device for uniform distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    for(int i = 0; i < n; i++){
        //Create ray
        Ray r;
        //Generate origin
        float r1 = dis(gen), r2 = dis(gen);
        r.origin = ((float)(1.0-sqrt(r1))) * triangle->p0 + ((float)(sqrt(r1)*(1.0-r2))) * triangle->p1 + ((float)(sqrt(r1) * r2)) * triangle->p2;
        //Generate direction
        r1 = dis(gen), r2 = dis(gen);
        glm::vec3 randPoint = ((float)(1.0-sqrt(r1))) * triangle->p0 + ((float)(sqrt(r1)*(1.0-r2))) * triangle->p1 + ((float)(sqrt(r1) * r2)) * triangle->p2;
        //Generate normal
        glm::vec3 normal = glm::normalize(glm::cross((triangle->p1 - triangle->p0), (triangle->p2 - triangle->p0)));
        glm::vec3 direction = randPoint + normal;

        //Set the direction
        r.direction = glm::normalize(direction - r.origin);

        //Update and push ray
        r.update();
        rays.push_back(r);
    }
    return rays;
};
