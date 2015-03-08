#include "kernel.hpp"
#include "types.hpp"

#include "util/thread_pool.hpp"
#include "util/vec3_helpers.hpp"

#include <tuple>
#include <iostream>

using namespace raytracer;

/**
Reflects ray 'r' in respect to the normal 'n'
**/
glm::vec3 reflect(glm::vec3 r, glm::vec3 n){
    return r-2*(glm::dot(r, n)/(glm::length(n)*glm::length(n)))*n;
}


glm::vec3 brdf(HitResult& hit, LightPtr ambient_light, std::vector<LightPtr> lights){

    glm::vec3 objCol = hit.shape->material->get_raw_color();

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 L;

    float ka = hit.shape->material->get_ka();
    float kd = hit.shape->material->get_kd();
    float ks = hit.shape->material->get_ks();
    float ke = hit.shape->material->get_ke();

    glm::vec3 r; r.x=1.0f; r.y = 0.0f; r.z = 0.0f;
    glm::vec3 n; n.x= 0.0f; r.y = 1.0f; r.z = 0.0f;

    reflect(r, n);


    //Ambient Component
    ambient = ka*objCol*ambient_light->color;

    //Calculate Diffuse and Specular Component
    for(int i = 0; i < lights.size(); i++){
        auto light = std::static_pointer_cast<DirectionalLight>(lights.at(i));

        //Diffuse
        glm::vec3 S = glm::normalize(hit.intersection_point - light->direction);
        diffuse = diffuse + (light->color * objCol * glm::dot(S, hit.intersection_normal));

        //Specular
        glm::vec3 specColor;
        specColor.r = 1.0f; specColor.g = 1.0f; specColor.b = 1.0f;

        //Need to get direction of mirror reflection
        glm::vec3 r = reflect(S , hit.intersection_normal);
        //specular = specular + (light->color * specColor) * pow(glm::dot(r, v), ke);
    }



    L = ambient + kd*diffuse;

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
