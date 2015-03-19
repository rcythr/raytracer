#pragma once

#include <string>
#include <glm/glm.hpp>
#include <stddef.h>

namespace raytracer {

enum class TextureFormat {
    RGB,
    RGBA
};

struct PNGTexture {
    
    PNGTexture() : data(nullptr) {}

    ~PNGTexture() { 
        if(data != nullptr)
            delete data; 
    }

    void load(std::string filename);

    bool mip_mapped;
    TextureFormat format;
    size_t width, height;
    unsigned char* data;

    glm::vec3 get_rgb(size_t row, size_t col);
    glm::vec4 get_rgba(size_t row, size_t col);
};

}
