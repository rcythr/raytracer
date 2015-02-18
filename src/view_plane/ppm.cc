
#include "view_plane/ppm.hpp"

using namespace raytracer;

PPMViewPlane::PPMViewPlane(size_t width, size_t height,
                           std::string output_filename)
    : ppm(width, height), filename(output_filename) {}

size_t PPMViewPlane::get_width() { return ppm.width; }

size_t PPMViewPlane::get_height() { return ppm.height; }

glm::vec3 PPMViewPlane::get_pixel(size_t row, size_t col) {
    return ppm.get_pixel(row, col);
}

void PPMViewPlane::set_pixel(size_t row, size_t col, glm::vec3 color) {
    ppm.set_pixel(row, col, color);
}

void PPMViewPlane::finish() { ppm.save(filename); }
