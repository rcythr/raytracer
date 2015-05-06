
#include "view_plane/ppm.hpp"

using namespace raytracer;

PPMViewPlane::PPMViewPlane(size_t width, size_t height,
                           std::string output_filename)
    : ppm(width, height), filename(output_filename) {}

size_t PPMViewPlane::get_width() { return ppm.width; }

size_t PPMViewPlane::get_height() { return ppm.height; }

glm::vec3 PPMViewPlane::get_pixel(size_t row, size_t col) {
    std::lock_guard<std::mutex> lg(mut);
    return ppm.get_pixel(row, col);
}

void PPMViewPlane::add_pixel(size_t row, size_t col, glm::vec3 color) {
    std::lock_guard<std::mutex> lg(mut);
    ppm.add_pixel(row, col, color);
}

void PPMViewPlane::set_pixel(size_t row, size_t col, glm::vec3 color) {
    std::lock_guard<std::mutex> lg(mut);
    ppm.set_pixel(row, col, color);
}

void PPMViewPlane::save() {
    ppm.save(filename);
}

void PPMViewPlane::finish(size_t num_samples) {
    std::lock_guard<std::mutex> lg(mut);

    for(size_t row = 0; row < ppm.height; ++row)
    {
        for(size_t col = 0; col < ppm.width; ++col)
        {
            ppm.set_pixel(row, col, ppm.get_pixel(row, col) / ((float)num_samples));
        }
    }
}
