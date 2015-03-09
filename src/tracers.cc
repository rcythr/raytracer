#include "kernel.hpp"
#include "types.hpp"

#include "util/thread_pool.hpp"
#include "util/vec3_helpers.hpp"

#include <math.h>

#include <tuple>
#include <iostream>

using namespace raytracer;

/**
 * Reflects ray 'r' in respect to the normal 'n'
 * @pre n vector is normalized.
**/
glm::vec3 reflect(glm::vec3 r, glm::vec3 n){ return glm::normalize(r - 2.0f * glm::dot(r, n) * n); }

glm::vec3 brdf(HitResult& hit, LightPtr ambient_light, std::vector<LightPtr> lights){

    auto& material = hit.shape->material;

    glm::vec3 objCol = material->get_raw_color();

    glm::vec3 result = material->ka() * ambient_light->color * objCol; 

    //Calculate Diffuse and Specular Component
    for(int i = 0; i < lights.size(); i++){
        if(lights[i]->get_type() == LightType::DIRECTION)
        {
            auto light = std::static_pointer_cast<DirectionalLight>(lights[i]);

            // For a directional light the vector to the light is the negation of it's direction.
            auto S = -light->direction;
            auto R = reflect(-light->direction, hit.intersection_normal);
            auto V = -hit.incoming_ray.direction;

            result += material->kd() * light->color * objCol * std::max(0.0f, glm::dot(S, hit.intersection_normal));
            result += material->ks() * light->color * objCol * std::pow(glm::dot(R, V), material->ke());
        }
    }

    return result;
}



void raytracer::checkpoint1(Kernel* kernel, CameraPtr camera) {
    // Now fire up a thread pool that does hit calculations and tone
    // reproduction.
    ThreadPool tp(kernel->num_threads);

    // Give the camera a function to call on each ray it generates.
    camera->spawn_rays([&](size_t row, size_t col, Ray& ray) { 
        tp.enqueue([=]() {
            kernel->spatial_index->find_closest_hit(ray, [=](HitResult& hit) {
                // Set the view plane pixel to the color of the
                // object we hit.
                glm::vec3 color = brdf(hit, kernel->ambient_light, kernel->lights);

                camera->view_plane->set_pixel(row, col, color);
            });

            if(col == 0)
            {
                std::cout << "\rCurrent Row = " << row << "            ";
            }
        });
    });

    // Now we wait for everyone to finish up.
    tp.stop();
    tp.join();

    // Signal to the view plane that we are finished writing.
    camera->view_plane->finish();

    std::cout << std::endl;
}
