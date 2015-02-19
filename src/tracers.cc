#include "kernel.hpp"
#include "types.hpp"

#include "util/thread_pool.hpp"
#include "util/vec3_helpers.hpp"

#include <tuple>

using namespace raytracer;

void raytracer::checkpoint1(Kernel* kernel, CameraPtr camera) {

    // First transform all objects into camera space for this camera.
    auto viewTransform = camera->build_transform_mat();
    kernel->spatial_index->view_all_objects([&](ShapePtr& shape) {
        shape->set_view_transform(viewTransform);
    });

    // Now fire up a thread pool that does hit calculations and tone reproduction.
    ThreadPool<std::tuple<size_t, size_t, Ray>> tp(
        kernel->num_threads, [=](std::tuple<size_t, size_t, Ray>& data) {
            size_t row = std::get<0>(data);
            size_t col = std::get<1>(data);

            kernel->spatial_index->find_closest_hit(std::get<2>(data),
                                                    [=](HitResult& hit) {
                // Set the view plane pixel to the color of the
                // object we hit.
                camera->view_plane->set_pixel(row, col, hit.shape->material->get_raw_color());
            });
        });

    // Give the camera a function to call on each ray it generates.
    camera->spawn_rays([&](size_t row, size_t col, Ray& ray) {
        tp.enqueue(std::tie(row, col, ray));
    });

    // Now we wait for everyone to finish up.
    tp.stop();
    tp.join();

    // Signal to the view plane that we are finished writing.
    camera->view_plane->finish();
}
