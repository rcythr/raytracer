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
glm::vec3 reflect(glm::vec3 L, glm::vec3 N)
{ 
    return glm::normalize(2.0f * glm::dot(N, L) * N - L);
}

glm::vec3 brdf(Kernel* kernel, HitResult& hit){

    auto& material = hit.shape->material;

    glm::vec3 objCol = material->get_raw_color();

    glm::vec3 result; 

    //Calculate Diffuse and Specular Component
    for(size_t i = 0; i < kernel->lights.size(); i++){
        auto type = kernel->lights[i]->get_type();
        if(type == LightType::DIRECTION)
        {
            auto light = std::static_pointer_cast<DirectionalLight>(kernel->lights[i]);

            // For a directional light the vector to the light is the negation of it's direction.
            auto S = -light->direction;

            Ray shadow_ray{hit.intersection_point, S};

            if(!kernel->spatial_index->has_hit(shadow_ray, hit.shape))
            {
                auto R = reflect(-S, hit.intersection_normal);
                auto V = -hit.incoming_ray.direction;

                result += material->kd() * objCol * std::max(0.0f, glm::dot(S, hit.intersection_normal));
                result += material->ks() * light->color * std::pow(glm::dot(R, V), material->ke());
            }
        }
        else if(type == LightType::POINT)
        {
            auto light = std::static_pointer_cast<PointLight>(kernel->lights[i]);

            auto S = glm::normalize(hit.intersection_point - light->point);

            Ray shadow_ray{hit.intersection_point, -S};
            if(!kernel->spatial_index->has_hit(shadow_ray, hit.shape))
            {
                auto R = reflect(-S, hit.intersection_normal);
                auto V = -hit.incoming_ray.direction;

                result += material->kd() * objCol * light->color * std::max(0.0f, glm::dot(-S, hit.intersection_normal));
                result += material->ks() * light->color * std::pow(std::max(0.0f, glm::dot(R, V)), material->ke());
            }
        }
        else if(type == LightType::AMBIENT)
        {
            auto light = std::static_pointer_cast<AmbientLight>(kernel->lights[i]);
            result += material->ka() * light->color * objCol;
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
                glm::vec3 color = brdf(kernel, hit);

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
