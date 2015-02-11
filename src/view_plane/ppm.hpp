#pragma once

#include "view_plane/view_plane.hpp"
#include "util/ppm/ppm.hpp"

#include <string>

namespace raytracer
{

struct PPMViewPlane : public ViewPlane
{
private:
    PPM ppm;
    std::string filename;
public:

    PPMViewPlane(size_t width, size_t height, std::string output_filename);

    size_t get_width() override;

    size_t get_height() override;

    glm::vec3 get_pixel(size_t row, size_t col) override;
    
    void set_pixel(size_t row, size_t col, glm::vec3 color) override;
    
    void finish() override;

};

}
