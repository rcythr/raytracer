
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

void PPMViewPlane::set_pixel(size_t row, size_t col, glm::vec3 color) {
    std::lock_guard<std::mutex> lg(mut);
    glm::vec3 cur_color = ppm.get_pixel(row, col);
    if (cur_color == glm::vec3(0.0f, 0.0f, 0.0f)) {
        ppm.set_pixel(row, col, color);
    } else {
        ppm.set_pixel(row, col, (cur_color + color) / 2.0f);
    }
}

void PPMViewPlane::finish() {
    std::lock_guard<std::mutex> lg(mut);
    ppm.apply_guassian();
    ppm.save(filename);
}
