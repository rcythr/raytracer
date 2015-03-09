#pragma once

#include <glm/glm.hpp>
#include <string>

namespace raytracer {

struct PPM {
    size_t width, height;
    glm::vec3** data;

    PPM(size_t width, size_t height);

    ~PPM();

    glm::vec3 get_pixel(size_t row, size_t col) { return data[row][col]; }

    void set_pixel(size_t row, size_t col, glm::vec3 color) {
        data[row][col] = color;
    }

    void apply_guassian();

    bool save(std::string filename);
};
}
