#include <sampler/png_texture.hpp>

using namespace raytracer;

glm::vec3 PNGTextureSampler::get_color(float u, float v) const {
    return texture->get_rgb(size_t(u * texture->height), size_t(v * texture->width));
}
