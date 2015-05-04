#pragma once

#include "material/phong.hpp"

#include <glm/glm.hpp>

namespace raytracer {
    
    struct PhotonMapped : public Phong {
        PhotonMapped(float ka, float kd, float ks, float ke, float kt, float ki, bool is_hollow)
            : Phong(ka, kd, ks, ke, kt, ki, is_hollow)
        { }

        glm::vec3 get_color(Kernel* kernel, HitResult& hit) override;
    };
}
