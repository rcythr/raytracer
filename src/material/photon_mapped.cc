#include "material/photon_mapped.hpp"
#include "kernel.hpp"

using namespace raytracer;

static float dist_squared(glm::vec3 a, glm::vec3 b)
{
    float x = a.x - b.x;
    float y = a.y - b.y;
    float z = a.z - b.z;
    return x*x + y*y + z*z;
}

glm::vec3 PhotonMapped::get_color(Kernel* kernel, HitResult& hit) {

    glm::vec3 indirect_result;
    glm::vec3 caustic_result;
    auto direct_result = Phong::get_color(kernel, hit);

    // Use the Global map to determine the indirect illumination.
    Photon p{ hit.intersection_point, hit.incoming_ray.direction, glm::vec3(0)};
    auto global_photons = kernel->global_photons.kNearestNeighbors(p, kernel->global_knn);

    float A = ((float)M_PI * dist_squared(global_photons[0].point, hit.intersection_point));

    for(size_t i=0; i < global_photons.size(); ++i)
    {
        auto& photon = global_photons[i];
        if(glm::dot(photon.incident_direction, -hit.intersection_normal) > 0)
            indirect_result += photon.power;
    }
    indirect_result /= A;

    // Use the caustics photon map to determine the additional caustic lighting
    auto caustic_photons = kernel->caustic_photons.kNearestNeighbors(p, kernel->caustic_knn);

    A = ((float)M_PI * dist_squared(caustic_photons[0].point, hit.intersection_point));

    for(size_t i=0; i < caustic_photons.size(); ++i)
    {
        auto& photon = caustic_photons[i];

        if(glm::dot(photon.incident_direction, -hit.intersection_normal) > 0)
            caustic_result += photon.power;
    }

    caustic_result /= A;

    return direct_result + indirect_result + caustic_result;
}
