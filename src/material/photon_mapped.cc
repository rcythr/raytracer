#include "material/photon_mapped.hpp"
#include "kernel.hpp"

#include "util/sample_hemisphere.hpp"
#include "util/random_utils.hpp"

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
    for(size_t i=0; i < kernel->final_gather_samples; ++i) {
        glm::vec3 result(0.0);
        Ray r{hit.intersection_point + 0.05f * hit.intersection_normal,
              sampleHemisphereCosine(hit.intersection_normal, uniform_random(), uniform_random())};
        kernel->spatial_index->find_closest_hit(r, [&](HitResult& hit){
            Photon p{ hit.intersection_point, hit.incoming_ray.direction, glm::vec3(0)};
            auto global_photons = hit.shape->global_photons.kNearestNeighbors(p, kernel->global_knn);
            
            float A = 1.0f;
            bool found_A = false;

            for(size_t i=0; i < global_photons.size(); ++i)
            {
                auto& photon = global_photons[i];
                
                float dot_product = glm::dot(photon.incident_direction, -hit.intersection_normal);
                if(dot_product > 0) 
                {
                    float dist = dist_squared(photon.point, hit.intersection_point);

                    if(!found_A) {
                            A = ((float) M_PI * dist);
                            found_A = true;
                    }

                    if(found_A) {
                        result += photon.color * photon.power;
                    }
                }
            }
            result *= kernel->global_exposure / A;
        });
        indirect_result += glm::dot(r.direction, hit.intersection_normal) * result;
    }

    // Use the caustics photon map to determine the additional caustic lighting
    Photon p{ hit.intersection_point, hit.incoming_ray.direction, glm::vec3(0)};
    auto caustic_photons = kernel->caustic_photons.kNearestNeighbors(p, kernel->caustic_knn);

    float A = 1.0f;
    bool found_A = false;

    for(size_t i=0; i < caustic_photons.size(); ++i)
    {
        auto& photon = caustic_photons[i];

        if(glm::dot(photon.incident_direction, -hit.intersection_normal) > 0)
        {
            float dist = dist_squared(photon.point, hit.intersection_point);

            if(!found_A) {
                A = ((float) M_PI * dist);
                found_A = true;
            }

            if(found_A) {
                caustic_result += photon.color * photon.power;
            }
        }
    }

    caustic_result *= kernel->caustic_exposure / A;

    return direct_result +indirect_result + caustic_result;
}
