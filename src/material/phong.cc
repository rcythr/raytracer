
#include "material/phong.hpp"

#include "kernel.hpp"

using namespace raytracer;

glm::vec3 Phong::get_color(Kernel* kernel, HitResult& hit) {
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

    return result;
}
