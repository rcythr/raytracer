#pragma once

#include "shape/shape.hpp"
#include "shape/triangle.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace raytracer
{

struct Tri { glm::vec3 p0, p1, p2; };

struct ObjMesh : public Shape
{
    ObjMesh(std::string filename, glm::mat4 transform, std::shared_ptr<Material> mat);

    std::string toString(size_t depth=0) override;

    bool test_hit(Ray& ray, double& tmin) override;
    
private:
    std::vector<Tri> triangles;
};

}
