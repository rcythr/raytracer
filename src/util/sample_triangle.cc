#include "util/sample_triangle.hpp"

glm::vec3 sampleTriangle(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, float r1, float r2)
{
    float sqrt_r1 = (float)sqrt(r1);
    return (1.0f-sqrt_r1) * p0 + 
           (sqrt_r1*(1.0f-r2)) * p1 + 
           (sqrt_r1 * r2) * p2;
}
