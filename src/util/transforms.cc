#include "util/transforms.hpp"

glm::mat4 raytracer::buildTranslation(glm::vec3 trans)
{
    return glm::mat4(
        glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
        glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
        glm::vec4(trans.x, trans.y, trans.z, 1.0f)
    );
}

glm::mat4 raytracer::buildRotationX(float theta)
{
    theta = M_PI / 180.0f * theta;

    float cos_theta = cos(theta);
    float sin_theta = sin(theta);

    return glm::mat4(
        glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec4(0.0f,  cos_theta, sin_theta, 0.0f),
        glm::vec4(0.0f, -sin_theta, cos_theta, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

glm::mat4 raytracer::buildRotationY(float theta)
{
    theta = M_PI / 180.0f * theta;
    
    float cos_theta = cos(theta);
    float sin_theta = sin(theta);
    
    return glm::mat4(
        glm::vec4(cos_theta, 0.0f, -sin_theta, 0.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
        glm::vec4(sin_theta, 0.0f, cos_theta, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

glm::mat4 raytracer::buildRotationZ(float theta)
{
    theta = M_PI / 180.0f * theta;
    
    float cos_theta = cos(theta);
    float sin_theta = sin(theta);
    
    return glm::mat4(
        glm::vec4(cos_theta, -sin_theta, 0.0f, 0.0f),
        glm::vec4(-sin_theta, cos_theta, 0.0f, 0.0f),
        glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

glm::mat4 raytracer::buildScale(float scale)
{
    return glm::mat4(
        glm::vec4(scale, 0.0f, 0.0f, 0.0f),
        glm::vec4(0.0f, scale, 0.0f, 0.0f),
        glm::vec4(0.0f, 0.0f, scale, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

