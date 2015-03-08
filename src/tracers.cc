#include "kernel.hpp"
#include "types.hpp"

#include "util/thread_pool.hpp"
#include "util/vec3_helpers.hpp"

#include <tuple>
#include <iostream>

using namespace raytracer;



/*glm::vec3 brdf(HitResult& hit, ){

    return glm::vec3 color;
}*/



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
                glm::vec3 color = hit.shape->material->get_raw_color();
                //glm::vec3 color = brdf(hit, kernal.ambient_light, kernel.lights);




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
