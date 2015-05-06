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
