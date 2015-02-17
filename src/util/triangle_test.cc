#include "util/triangle_test.hpp"

bool raytracer::test_triangle_hit(Ray& ray, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, double& tmin)
{
    float a, f, u, v, t;
    glm::vec3 e1, e2, h, s, q;

    e1 = p1 - p0;
    e2 = p2 - p0;

    h = glm::cross(ray.direction, e2);
    a = glm::dot(e1, h);

    if(a > -0.00001 && a < 0.00001)
        return false;

    f = 1/a;

    s = ray.origin - p0;
    u = f * glm::dot(s, h);

    if(u < 0.0 || u > 1.0)
        return false;

    q = glm::cross(s, e1);

    v = f * glm::dot(ray.direction, q);

    if(v < 0.0 || u + v > 1.0)
        return false;

    t = f * glm::dot(e2, q);

    if (t > 0.00001)
    {
        tmin = t;
        return true;
    }
    return false;
}
