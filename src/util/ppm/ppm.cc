#include "util/ppm/ppm.hpp"

#include <iostream>
#include <sstream>
#include <cstdio>

using namespace raytracer;

PPM::PPM(int64_t width, int64_t height) : width(width), height(height) {
    data = new glm::vec3* [height];
    for (int64_t i = 0; i < height; ++i) {
        data[i] = new glm::vec3[width];
    }
}

PPM::~PPM() {
    if (data != nullptr) {
        for (int64_t i = 0; i < height; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }
}

void PPM::map()
{
    glm::vec3 min = data[0][0];
    glm::vec3 max = data[0][0];

    // First determine our range.
    for(int64_t i=0; i < (int64_t)height; ++i) {
        for(int64_t j=0; j < (int64_t)width; ++j) {
            auto& v = data[i][j];

            if(v.r < 0.0f) v.r = 0.0f;
            if(v.g < 0.0f) v.g = 0.0f;
            if(v.b < 0.0f) v.b = 0.0f;
            
            if(v.r > 1.0f) v.r = 1.0f;
            if(v.g > 1.0f) v.g = 1.0f;
            if(v.b > 1.0f) v.b = 1.0f;

            if(v.r < min.r) min.r = v.r;
            if(v.r > max.r) max.r = v.r;

            if(v.g < min.g) min.g = v.g;
            if(v.g > max.g) max.g = v.g;
            
            if(v.b < min.b) min.b = v.b;
            if(v.b > max.b) max.b = v.b;
        }
    }

    std::cout << '(' << min.r << ',' <<  min.g << ',' << min.b  << ')' << std::endl;
    std::cout << '(' << max.r << ',' <<  max.g << ',' << max.b  << ')' << std::endl;

    // Now calculate the linear equation for each color and scale
    for(int64_t i=0; i < (int64_t)height; ++i) {
        for(int64_t j=0; j < (int64_t)width; ++j) {
            auto& v = data[i][j];
            v.r /= max.r;
            v.g /= max.g;
            v.b /= max.b;
        }
    }
}

void PPM::apply_guassian() {
    glm::vec3 v11, v12, v13, v21, v22, v23, v31, v32, v33;

    glm::vec3 zero;

    // Create temporary storage for the output data.
    glm::vec3** out_data = new glm::vec3* [height];
    for (int64_t i = 0; i < (int64_t)height; ++i) {
        out_data[i] = new glm::vec3[width];
    }

    // Apply filter
    for (int64_t i = 0; i < (int64_t)height; ++i) {
        for (int64_t j = 0; j < (int64_t)width; ++j) {
            v11 = (i - 1 >= 0 && j - 1 >= 0) ? 0.0625f * data[i - 1][j - 1]
                                             : zero;
            v12 = (i - 1 >= 0) ? 0.128f * data[i - 1][j] : zero;
            v13 = (i - 1 >= 0 && j + 1 < width) ? 0.0625f * data[i - 1][j + 1]
                                                : zero;

            v21 = (j - 1 >= 0) ? 0.128f * data[i][j - 1] : zero;
            v22 = 0.256f * data[i][j];
            v23 = (j + 1 < width) ? 0.128f * data[i][j + 1] : zero;

            v31 = (i + 1 < height && j - 1 >= 0) ? 0.0625f * data[i + 1][j - 1]
                                                 : zero;
            v32 = (i + 1 < height) ? 0.128f * data[i + 1][j] : zero;
            v33 = (i + 1 < height && j + 1 < width)
                      ? 0.0625f * data[i + 1][j + 1]
                      : zero;

            out_data[i][j] =
                v11 + v12 + v13 + v21 + v22 + v23 + v31 + v32 + v33;
        }
    }

    // Delete the old data array.
    for (int64_t i = 0; i < (int64_t)height; ++i) {
        delete[] data[i];
    }
    delete[] data;

    // Replace it with the new data array.
    data = out_data;
}

bool PPM::save(std::string filename) {
    // Generate the proper Header
    std::stringstream ss;
    ss << "P6" << std::endl;
    ss << width << " " << height << "\n";
    ss << 255 << "\n";
    std::string header = ss.str();

    // Open a file descriptor to the output file
    std::FILE* fd = std::fopen(filename.c_str(), "wb");
    if (fd == nullptr) {
        return false;
    }

    // Write out the header
    std::fwrite(header.c_str(), sizeof(char), header.size(), fd);

    // Write out all of the pixel data.
    for (int64_t i = 0; i < (int64_t)height; ++i) {
        for (int64_t j = 0; j < (int64_t)width; ++j) {
            auto& pix = data[i][j];

            int32_t r = std::min(std::max((int32_t)(pix.r * 255), 0), 255);
            int32_t g = std::min(std::max((int32_t)(pix.g * 255), 0), 255);
            int32_t b = std::min(std::max((int32_t)(pix.b * 255), 0), 255);

            std::fputc((uint8_t)r, fd);
            std::fputc((uint8_t)g, fd);
            std::fputc((uint8_t)b, fd);
        }
    }

    // Close the stream
    std::fclose(fd);

    return true;
}
