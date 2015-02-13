#include "kernel.hpp"
#include "types.hpp"

#include "util/vec3_helpers.hpp"

using namespace raytracer;

void raytracer::checkpoint1(Kernel* kernel)
{

    // Give the camera a function to call on each ray it generates. 
    kernel->camera->spawn_rays([=](size_t row, size_t col, Ray& ray) {
        kernel->spatial_index->find_closest_hit(ray, [=](ShapePtr& hitShape, double tmin) {
            // Set the view plane pixel to the color of the object we hit.
            kernel->camera->view_plane->set_pixel(row, col, hitShape->material->get_raw_color());
        });
    });

    // Signal to the view plane that we are finished writing.
    kernel->camera->view_plane->finish();
}
