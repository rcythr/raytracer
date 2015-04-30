
#include "shape/triangle.hpp"
#include "material/material.hpp"

#include "util/vec3_helpers.hpp"

using namespace raytracer;

template <typename T> int sign(T val) { return (T(0) < val) - (val < T(0)); }

void Triangle::test_hit(const Ray& ray, HitResult& result) {
    glm::vec3 e1, e2;

    e1 = p1 - p0;
    e2 = p2 - p0;

    auto T = ray.origin - p0;
    auto P = glm::cross(ray.direction, e2);
    auto Q = glm::cross(T, e1);

    float p_dot_e1 = glm::dot(P, e1);

    if (p_dot_e1 == 0.0f) {
        result.miss();
        return;
    }

    float t = glm::dot(Q, e2) / p_dot_e1;
    if (t < 0.0f) {
        result.miss();
        return;
    }

    float u = glm::dot(P, T) / p_dot_e1;
    float v = glm::dot(Q, ray.direction) / p_dot_e1;

    if (u < 0.0f || v < 0.0f || (u + v) > 1.0f) {
        result.miss();
        return;
    }

    // Now calculate UV mapping
    auto intersection_point = ((1.0f - u - v) * p0) + (u * p1) + (v * p2);
    auto f1 = p0 - intersection_point;
    auto f2 = p1 - intersection_point;
    auto f3 = p2 - intersection_point;

    auto va = glm::cross(p0 - p1, p0 - p2);
    auto va1 = glm::cross(f2, f3);
    auto va2 = glm::cross(f3, f1);
    auto va3 = glm::cross(f1, f2);

    auto a = glm::length(va);

    auto a1 = glm::length(va1) / a * sign(glm::dot(va, va1));
    auto a2 = glm::length(va2) / a * sign(glm::dot(va, va2));
    auto a3 = glm::length(va3) / a * sign(glm::dot(va, va3));

    auto uv = uv0 * a1 + uv1 * a2 + uv2 * a3;

    result.hit(shared_from_this(), t, uv.x, uv.y, intersection_point,
               glm::normalize(glm::cross(e1, e2)), ray);
}

// Code adapted from
// http://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/tribox3.txt

#define X 0
#define Y 1
#define Z 2

inline bool plane_box_overlap(glm::vec3 normal, const glm::vec3 vert,
                              const glm::vec3 maxbox) {
    glm::vec3 vmin, vmax;
    float v;
    for (size_t i = X; i <= Z; i++) {
        v = vert[i];
        if (normal[i] > 0.0f) {
            vmin[i] = -maxbox[i] - v;
            vmax[i] = maxbox[i] - v;
        } else {
            vmin[i] = maxbox[i] - v;
            vmax[i] = -maxbox[i] - v;
        }
    }
    if (glm::dot(normal, vmin) >= 0.0f)
        return false;

    if (glm::dot(normal, vmax) >= 0.0f)
        return true;

    return false;
}

inline void find_min_max(float x0, float x1, float x2, float& min, float& max) {
    min = max = x0;
    if (x1 < min)
        min = x1;
    if (x1 > max)
        max = x1;
    if (x2 < min)
        min = x2;
    if (x2 > max)
        max = x2;
}

/*======================== X-tests ========================*/

#define AXISTEST_X01(a, b, fa, fb)                                             \
    p[0] = a * v0[Y] - b * v0[Z];                                              \
    p[2] = a * v2[Y] - b * v2[Z];                                              \
    if (p[0] < p[2]) {                                                         \
        min = p[0];                                                            \
        max = p[2];                                                            \
    } else {                                                                   \
        min = p[2];                                                            \
        max = p[0];                                                            \
    }                                                                          \
    rad = fa * box_halfsize[Y] + fb * box_halfsize[Z];                         \
    if (min > rad || max < -rad)                                               \
        return 0;

#define AXISTEST_X2(a, b, fa, fb)                                              \
    p[0] = a * v0[Y] - b * v0[Z];                                              \
    p[1] = a * v1[Y] - b * v1[Z];                                              \
    if (p[0] < p[1]) {                                                         \
        min = p[0];                                                            \
        max = p[1];                                                            \
    } else {                                                                   \
        min = p[1];                                                            \
        max = p[0];                                                            \
    }                                                                          \
    rad = fa * box_halfsize[Y] + fb * box_halfsize[Z];                         \
    if (min > rad || max < -rad)                                               \
        return 0;

/*======================== Y-tests ========================*/

#define AXISTEST_Y02(a, b, fa, fb)                                             \
    p[0] = -a * v0[X] + b * v0[Z];                                             \
    p[2] = -a * v2[X] + b * v2[Z];                                             \
    if (p[0] < p[2]) {                                                         \
        min = p[0];                                                            \
        max = p[2];                                                            \
    } else {                                                                   \
        min = p[2];                                                            \
        max = p[0];                                                            \
    }                                                                          \
    rad = fa * box_halfsize[X] + fb * box_halfsize[Z];                         \
    if (min > rad || max < -rad)                                               \
        return 0;

#define AXISTEST_Y1(a, b, fa, fb)                                              \
    p[0] = -a * v0[X] + b * v0[Z];                                             \
    p[1] = -a * v1[X] + b * v1[Z];                                             \
    if (p[0] < p[1]) {                                                         \
        min = p[0];                                                            \
        max = p[1];                                                            \
    } else {                                                                   \
        min = p[1];                                                            \
        max = p[0];                                                            \
    }                                                                          \
    rad = fa * box_halfsize[X] + fb * box_halfsize[Z];                         \
    if (min > rad || max < -rad)                                               \
        return 0;

/*======================== Z-tests ========================*/
#define AXISTEST_Z12(a, b, fa, fb)                                             \
    p[1] = a * v1[X] - b * v1[Y];                                              \
    p[2] = a * v2[X] - b * v2[Y];                                              \
    if (p[2] < p[1]) {                                                         \
        min = p[2];                                                            \
        max = p[1];                                                            \
    } else {                                                                   \
        min = p[1];                                                            \
        max = p[2];                                                            \
    }                                                                          \
    rad = fa * box_halfsize[X] + fb * box_halfsize[Y];                         \
    if (min > rad || max < -rad)                                               \
        return 0;

#define AXISTEST_Z0(a, b, fa, fb)                                              \
    p[0] = a * v0[X] - b * v0[Y];                                              \
    p[1] = a * v1[X] - b * v1[Y];                                              \
    if (p[0] < p[1]) {                                                         \
        min = p[0];                                                            \
        max = p[1];                                                            \
    } else {                                                                   \
        min = p[1];                                                            \
        max = p[0];                                                            \
    }                                                                          \
    rad = fa * box_halfsize[X] + fb * box_halfsize[Y];                         \
    if (min > rad || max < -rad)                                               \
        return 0;

bool Triangle::test_hit(const AABB& aabb) {
    glm::vec3 v0, v1, v2, e0, e1, e2, p;
    float min, max, rad, fex, fey, fez;

    auto box_center = aabb.center();
    auto box_halfsize = aabb.half_size();

    v0 = p0 - box_center;
    v1 = p1 - box_center;
    v2 = p2 - box_center;

    e0 = v1 - v0;
    e1 = v2 - v1;
    e2 = v0 - v2;

    fex = std::abs(e0[X]);
    fey = std::abs(e0[Y]);
    fez = std::abs(e0[Z]);
    AXISTEST_X01(e0[Z], e0[Y], fez, fey);
    AXISTEST_Y02(e0[Z], e0[X], fez, fex);
    AXISTEST_Z12(e0[Y], e0[X], fey, fex);

    fex = std::abs(e1[X]);
    fey = std::abs(e1[Y]);
    fez = std::abs(e1[Z]);
    AXISTEST_X01(e1[Z], e1[Y], fez, fey);
    AXISTEST_Y02(e1[Z], e1[X], fez, fex);
    AXISTEST_Z0(e1[Y], e1[X], fey, fex);

    fex = std::abs(e2[X]);
    fey = std::abs(e2[Y]);
    fez = std::abs(e2[Z]);
    AXISTEST_X2(e2[Z], e2[Y], fez, fey);
    AXISTEST_Y1(e2[Z], e2[X], fez, fex);
    AXISTEST_Z12(e2[Y], e2[X], fey, fex);

    find_min_max(v0[X], v1[X], v2[X], min, max);
    if (min > box_halfsize[X] || max < -box_halfsize[X])
        return false;

    find_min_max(v0[Y], v1[Y], v2[Y], min, max);
    if (min > box_halfsize[Y] || max < -box_halfsize[Y])
        return false;

    find_min_max(v0[Z], v1[Z], v2[Z], min, max);
    if (min > box_halfsize[Z] || max < -box_halfsize[Z])
        return false;

    if (!plane_box_overlap(glm::cross(e0, e1), v0, box_halfsize))
        return false;

    return true;
}
