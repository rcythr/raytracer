
#include "png.hpp"

#include <zlib.h>
#include <png.h>

#include <stdexcept>

using namespace raytracer;

void PNGTexture::load(std::string filename)
{
    FILE* infile = fopen(filename.c_str(), "rb");
    if(!infile)
    {
        throw std::runtime_error("File supplied for PNG texture loading does not exist.");
    }

    // Check magic number
    unsigned char sig[8];
    fread(sig, 1, 8, infile);
    if(png_sig_cmp(sig, 0, 8)) {
        fclose(infile);
        throw std::runtime_error("File supplied for PNG texture loading is invalid or corrupt.");
    }

    /// Begin PNG introductory housekeeping.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if(!png_ptr)
    {
        fclose(infile);
        throw std::runtime_error("Out of memory while loading PNG texture.");
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(infile);
        throw std::runtime_error("Out of memory while loading PNG texture.");
    }

    if(setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(infile);
        throw std::runtime_error("Error while loading PNG.");
    }
    /// End PNG Housekeeping.

    png_init_io(png_ptr, infile);
    png_set_sig_bytes(png_ptr, 8);

    png_read_png(png_ptr, info_ptr,
        PNG_TRANSFORM_STRIP_16 |
        PNG_TRANSFORM_PACKING |
        PNG_TRANSFORM_EXPAND,
        nullptr);

    /// Determine the format of the image.
    TextureFormat format;
    switch(png_get_channels(png_ptr, info_ptr))
    {
    case 4: format = TextureFormat::RGBA; break;
    case 3: format = TextureFormat::RGB; break;
    default: throw std::runtime_error("Unsupported number of color channels in PNG image.");
    }

    /// Prep some counters and variables.
    unsigned int rowBytes = png_get_rowbytes(png_ptr, info_ptr);
    png_bytepp row_ptrs = png_get_rows(png_ptr, info_ptr);
    size_t height = png_get_image_height(png_ptr, info_ptr);
    size_t width = png_get_image_width(png_ptr, info_ptr);

    /// Prepare the output.
    this->format = format;
    this->height = height;
    this->width = width;
    this->data = new unsigned char[height * rowBytes];

    /// Transfer libpng buffer data to our buffer.
    unsigned char* offset = this->data + rowBytes * (height-1);
    for(size_t i=0; i < height; ++i)
    {
        memcpy(offset, row_ptrs[i], rowBytes);
        offset -= rowBytes;
    }

    /// Cleanup.
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(infile);
}

glm::vec3 PNGTexture::get_rgb(size_t row, size_t col)
{
    size_t row_bytes;
    unsigned char* ptr;
    switch(format) {
        case TextureFormat::RGB: 
            row_bytes = width * 3;
            ptr = data + row_bytes * row + (col * 3);
            return glm::vec3(float(ptr[0])/256.0f, float(ptr[1])/256.0f, float(ptr[2])/256.0f);
        case TextureFormat::RGBA:
            row_bytes = width * 4;
            ptr = data + row_bytes * row + (col * 4);
            return glm::vec3(float(ptr[0])/256.0f, float(ptr[1])/256.0f, float(ptr[2])/256.0f);
    }
}

glm::vec4 PNGTexture::get_rgba(size_t row, size_t col)
{
    size_t row_bytes;
    unsigned char* ptr;
    switch(format) {
        case TextureFormat::RGB:
            row_bytes = width * 3;
            ptr = data + row_bytes * row + (col * 3);
            return glm::vec4(float(ptr[0])/256.0f, float(ptr[1])/256.0f, float(ptr[2])/256.0f, 1.0f);
        case TextureFormat::RGBA:
            row_bytes = width * 4;
            ptr = data + row_bytes * row + (col * 4);
            return glm::vec4(float(ptr[0])/256.0f, float(ptr[1])/256.0f, float(ptr[2])/256.0f, float(ptr[3])/256.0f);
    }
}
