#include "kernel.hpp"
#include "types.hpp"

#include "util/thread_pool.hpp"
#include "util/vec3_helpers.hpp"

#include <math.h>

#include <tuple>
#include <iostream>

using namespace raytracer;

void raytracer::checkpoint1(Kernel* kernel, CameraPtr camera) {
    // Now fire up a thread pool that does hit calculations and tone
    // reproduction.
    ThreadPool tp(kernel->num_threads);

    // Give the camera a function to call on each ray it generates.
    camera->spawn_rays([&](size_t row, size_t col, Ray& ray) { 
        tp.enqueue([=]() {
            bool hit_found = false;

            kernel->spatial_index->find_closest_hit(ray, [=,&hit_found](HitResult& hit) {
                camera->view_plane->set_pixel(row, col, hit.shape->material->get_color(kernel, hit));
                hit_found = true;
            });

            if(!hit_found) {
                camera->view_plane->set_pixel(row, col, kernel->background_color);
            }

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
