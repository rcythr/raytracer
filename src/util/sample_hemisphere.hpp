#pragma once

glm::vec3 findPerpendicular(glm::vec3 vec)
{
    glm::vec3 abs_vec = glm::abs(vec);
    glm::vec3 min_vec = glm::vec3(0.0f, 0.0f, 1.0f);
    if(abs_vec.x < abs_vec.y)
    {
        if(abs_vec.z > abs_vec.x)
        {
            min_vec = glm::vec3(1.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        if(abs_vec.z > abs_vec.y) 
        {
            min_vec = glm::vec3(0.0f, 1.0f, 0.0f);
        }
    }
    return glm::cross(min_vec, vec);
}

glm::vec3 sampleHemisphere(glm::vec3& normal, float Xi1, float Xi2)
{
    float t = 2.0f * M_PI * Xi2;
    float s = sqrtf(1.0f - (Xi1 * Xi1));
    float x = s * cosf(t);
    float y = s * sinf(t);

    glm::vec3 e1 = findPerpendicular(normal); 
    glm::vec3 e2 = glm::cross(e1, normal);

    return glm::normalize(glm::vec3(x) * e1 + glm::vec3(y) * e2 + glm::vec3(Xi1) * normal);
}
