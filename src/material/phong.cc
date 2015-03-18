
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

    return ss.str();
}

/**
 * Reflects ray 'r' in respect to the normal 'n'
 * @pre n vector is normalized.
**/
glm::vec3 reflect(glm::vec3 L, glm::vec3 N)
{ 
    return glm::normalize(2.0f * glm::dot(N, L) * N - L);
}

glm::vec3 Phong::get_color(Kernel* kernel, HitResult& hit) 
{
    auto color = sampler->get_color(hit.u, hit.v);
    
    glm::vec3 result; 

    //Calculate Diffuse and Specular Component
    for(size_t i = 0; i < kernel->lights.size(); i++){
        auto type = kernel->lights[i]->get_type();
        if(type == LightType::DIRECTION)
        {
            auto light = std::static_pointer_cast<DirectionalLight>(kernel->lights[i]);

            // For a directional light the vector to the light is the negation of it's direction.
            auto S = -light->direction;

            Ray shadow_ray{hit.intersection_point, S};

            if(!kernel->spatial_index->has_hit(shadow_ray, hit.shape))
            {
                auto R = reflect(S, hit.intersection_normal);
                auto V = -hit.incoming_ray.direction;

                result += kd * color * std::max(0.0f, glm::dot(S, hit.intersection_normal));
                result += ks * light->color * std::pow(glm::dot(R, V), ke);
            }
        }
        else if(type == LightType::POINT)
        {
            auto light = std::static_pointer_cast<PointLight>(kernel->lights[i]);

            auto S = glm::normalize(light->point - hit.intersection_point);

            Ray shadow_ray{hit.intersection_point, S};
            if(!kernel->spatial_index->has_hit(shadow_ray, hit.shape))
            {
                auto R = reflect(S, hit.intersection_normal);
                auto V = -hit.incoming_ray.direction;

                result += kd * color * light->color * std::max(0.0f, glm::dot(S, hit.intersection_normal));
                result += ks * light->color * std::pow(std::max(0.0f, glm::dot(R, V)), ke);
            }
        }
        else if(type == LightType::AMBIENT)
        {
            auto light = std::static_pointer_cast<AmbientLight>(kernel->lights[i]);
            result += ka * light->color * color;
        }
    }

    return result;
}

