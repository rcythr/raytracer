
#include "material/phong.hpp"

#include "kernel.hpp"

#include "util/sample_triangle.hpp"

using namespace raytracer;

static thread_local std::random_device rd;
static thread_local std::mt19937 gen(rd());
static thread_local std::uniform_real_distribution<> dis(0, 1);

glm::vec3 Phong::get_color(Kernel* kernel, HitResult& hit) {
    auto color = sampler->get_color(hit.u, hit.v);

    glm::vec3 result;

    // Calculate Direct Diffuse and Specular Component from Light Sources
    for (size_t i = 0; i < kernel->lights.size(); i++) {
        switch(kernel->lights[i]->get_type())
        {
        case LightType::DIRECTION:
        {
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
        } break;
        case LightType::POINT:
        {
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
        } break; 
        case LightType::AMBIENT:
        {
            auto light = std::static_pointer_cast<AmbientLight>(kernel->lights[i]);

            // Add in ambient component.
            result += ka * light->color * color;
        } break;
        case LightType::TRIANGULAR:
        {
            auto light = std::static_pointer_cast<TriangularLight>(kernel->lights[i]);

            // We will use 10 samples on the light source to try and see if it's occluded.
            // We consider the 3 edges and 7 internal points.
            std::vector<glm::vec3> points;

            for(size_t i=0; i < 5; ++i)
            {
                points.push_back(sampleTriangle(light->triangle->p0,
                                                light->triangle->p1,
                                                light->triangle->p2,
                                                dis(gen),
                                                dis(gen)));
            }

            for(auto& pt : points)
            {
                float power = light->intensity / (pow(pt.x - hit.intersection_point.x, 2) + pow(pt.y - hit.intersection_point.y, 2) + pow(pt.z - hit.intersection_point.z, 2));
                auto dir = glm::normalize(pt - hit.intersection_point);

                Ray shadow_ray{hit.intersection_point, dir};
                shadow_ray.update();

                if(!kernel->spatial_index->has_hit(shadow_ray, hit.shape))
                {
                    auto R = glm::reflect(-dir, hit.intersection_normal);
                    auto V = -hit.incoming_ray.direction;

                    // Add in Diffuse Component
                    result += 0.2f * power * kd * color * light->color * std::max(0.0f, glm::dot(dir, hit.intersection_normal));

                    // Add in Specular component
                    result += 0.2f * power * ks * light->color * std::pow(std::max(0.0f, glm::dot(R, V)), ke);
                }
            }

        } break;
        }
    }

    return result;
}
