#include "kernel.hpp"
#include "types.hpp"

#include "util/thread_pool.hpp"
#include "util/vec3_helpers.hpp"

#include <tuple>
#include <iostream>

using namespace raytracer;


glm::vec3 brdf(HitResult& hit, LightPtr ambient_light, std::vector<LightPtr> lights){

    glm::vec3 objCol = hit.shape->material->get_raw_color();

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 L;

    float ka = hit.shape->material->get_ka();
    //float kd = hit.shape->material->get_kd();
    //float ks = hit.shape->material->get_ks();
    //float ke = hit.shape->material->get_ke();


    //Ambient Component
    ambient = ka*objCol*ambient_light->color;

    //Calculate Diffuse Component
    for(int i = 0; i < lights.size(); i++){
        LightPtr light = lights.at(i);
        //glm::vec3 S = glm::normalize(hit.intersection_point - light->get_direction());
        diffuse = diffuse + (light->color * objCol);
    }

    L = ambient + diffuse;

    return L;
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
