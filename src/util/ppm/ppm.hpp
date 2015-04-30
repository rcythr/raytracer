#pragma once

#include <glm/glm.hpp>
#include <string>

namespace raytracer {

struct PPM {
    int64_t width, height;
    glm::vec3** data;

    PPM(int64_t width, int64_t height);

    ~PPM();

    glm::vec3 get_pixel(int64_t row, int64_t col) { return data[row][col]; }

    void add_pixel(int64_t row, int64_t col, glm::vec3 color) { data[row][col] += color; }

    void set_pixel(int64_t row, int64_t col, glm::vec3 color) {
        data[row][col] = color;
    }

    void map();

    void apply_guassian();

    bool save(std::string filename);
};
}
