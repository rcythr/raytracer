
#include "material/phong.hpp"

#include "kernel.hpp"

#include <sstream>
#include "util/string_mult.hpp"

using namespace raytracer;

std::string Phong::toString(size_t depth) {
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;
    ss << tabdepth << "TYPE: MATTE\n";
    ss << tabdepth << "KA: " << ka << "\n";
    ss << tabdepth << "KD: " << kd << "\n";
    ss << tabdepth << "KS: " << ks << "\n";
    ss << tabdepth << "KE: " << ke << "\n";
    ss << tabdepth << "KR: " << kr << "\n";
    ss << tabdepth << "KT: " << kt << "\n";
    ss << tabdepth << "KI: " << ki << "\n";

    return ss.str();
}

glm::vec3 Phong::get_color(Kernel* kernel, HitResult& hit, size_t num_bounces, size_t max_bounces) {
    auto color = sampler->get_color(hit.u, hit.v);

    glm::vec3 result;

    // Calculate Direct Diffuse and Specular Component from Light Sources
    for (size_t i = 0; i < kernel->lights.size(); i++) {
        auto type = kernel->lights[i]->get_type();
        if (type == LightType::DIRECTION) {
            auto light =
                std::static_pointer_cast<DirectionalLight>(kernel->lights[i]);

            // For a directional light the vector to the light is the negation
            // of it's direction.
            auto S = -light->direction;

            Ray shadow_ray{ hit.intersection_point, S };

            shadow_ray.update();

            if (!kernel->spatial_index->has_hit(shadow_ray, hit.shape)) {
                auto R = glm::reflect(-S, hit.intersection_normal);
                auto V = -hit.incoming_ray.direction;

                // Add in Diffuse Component
                result += kd * color * std::max(0.0f, glm::dot(S, hit.intersection_normal));

                // Add in Specular Component
                result += ks * light->color * std::pow(glm::dot(R, V), ke);
            }
        } else if (type == LightType::POINT) {
            auto light =
                std::static_pointer_cast<PointLight>(kernel->lights[i]);

            auto S = glm::normalize(light->point - hit.intersection_point);

            Ray shadow_ray{ hit.intersection_point, S };
            shadow_ray.update();
            if (!kernel->spatial_index->has_hit(shadow_ray, hit.shape)) {
                auto R = glm::reflect(-S, hit.intersection_normal);
                auto V = -hit.incoming_ray.direction;

                // Add in Diffuse Component
                result += kd * color * light->color * std::max(0.0f, glm::dot(S, hit.intersection_normal));

                // Add in Specular component
                result += ks * light->color * std::pow(std::max(0.0f, glm::dot(R, V)), ke);
            }
        } else if (type == LightType::AMBIENT) {
            auto light = std::static_pointer_cast<AmbientLight>(kernel->lights[i]);

            // Add in ambient component.
            result += ka * light->color * color;
        }
    }

    if(num_bounces != max_bounces)
    {
        Ray refl{hit.intersection_point, glm::reflect(hit.incoming_ray.direction, hit.intersection_normal)};
        refl.update();
        result += kr * kernel->get_color_rec(refl, num_bounces + 1, max_bounces, hit.shape);

        Ray trans{hit.intersection_point, hit.incoming_ray.direction}; 
        trans.update();
        result += kt * kernel->get_color_rec(trans, num_bounces + 1, max_bounces, nullptr);
    }

    return result;
}
