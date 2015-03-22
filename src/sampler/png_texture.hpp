#pragma once

#include <sampler/sampler.hpp>
#include <util/png/png.hpp>
#include <memory>
#include <utility>

namespace raytracer {

struct PNGTextureSampler : public Sampler {

    PNGTextureSampler(std::unique_ptr<PNGTexture>&& texture)
        : texture(std::forward<std::unique_ptr<PNGTexture> >(texture)) {}

    glm::vec3 get_color(float u, float v) const override;

    std::unique_ptr<PNGTexture> texture;
};
}
