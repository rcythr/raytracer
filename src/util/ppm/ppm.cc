#include "util/ppm/ppm.hpp"

#include <sstream>
#include <cstdio>

using namespace raytracer;

PPM::PPM(size_t width, size_t height)
    : width(width)
    , height(height)
{
    data = new glm::vec3*[height];
    for(size_t i=0; i < height; ++i)
    {
        data[i] = new glm::vec3[width];
    }
}

PPM::~PPM()
{
    if(data != nullptr)
    {
        for(size_t i=0; i < height; ++i)
        {
            delete[] data[i];
        }
        delete[] data;
    }
}

bool PPM::save(std::string filename) 
{
    // Generate the proper Header
    std::stringstream ss;
    ss << "P6" << std::endl;
    ss << width << " " << height << "\n";
    ss << 255 << "\n"; 
    std::string header = ss.str();

    // Open a file descriptor to the output file
    std::FILE* fd = std::fopen(filename.c_str(), "wb");
    if(fd == nullptr)
    {
        return false;
    }

    // Write out the header
    std::fwrite(header.c_str(), sizeof(char), header.size(), fd);

    // Write out all of the pixel data.
    for(size_t i=height; i > 0; --i)
    {
        for(size_t j=0; j < width; ++j)
        {
            auto& pix = data[i-1][j];

            std::fputc((uint8_t)(pix.r * 255), fd);
            std::fputc((uint8_t)(pix.g * 255), fd);
            std::fputc((uint8_t)(pix.b * 255), fd);
        }
    }

    // Close the stream
    std::fclose(fd);

    return true;
}
