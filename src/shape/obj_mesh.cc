#include "shape/obj_mesh.hpp"
#include "material/material.hpp"

#include <sstream>
#include <fstream>

#include "util/string_mult.hpp"
#include "util/split.hpp"
#include "util/triangle_test.hpp"

using namespace raytracer;

void loadObj(std::string filename, glm::mat4 transform, std::vector<Tri>& outTriangles);

ObjMesh::ObjMesh(std::string filename, glm::mat4 transform, std::shared_ptr<Material> mat)
    : Shape(mat)
{
    loadObj(filename, transform, triangles);
}

std::string ObjMesh::toString(size_t depth)
{
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;
    ss << tabdepth << "TYPE: OBJ MESH\n";
    ss << tabdepth << "#TRIS: " << triangles.size() << "\n";
    ss << tabdepth << "MATERIAL: \n";
    ss << material->toString(depth+1);

    return ss.str();
}

void ObjMesh::test_hit(Ray& ray, HitResult& result)
{
    bool found = false;
    size_t found_tri;
    double t;

    size_t num_triangles = triangles.size();
    for(size_t i=0; i < num_triangles; ++i)
    {
        double newt;
        if(test_triangle_hit(ray, triangles[i].p0, triangles[i].p1, triangles[i].p2, newt))
        {
            if(!found || newt < t)
            {
                found = true;
                found_tri = i;
                t = newt;
            }
        }
    }

    if(found)
    {
        tmin = t;
        return true;
    }

    return false;
}

void loadObj(std::string filename, glm::mat4 transform, std::vector<Tri>& outTriangles)
{
    std::ifstream strm(filename.c_str());
    std::vector<glm::vec3> points;
    std::string line;

    while(std::getline(strm, line))
    {
        auto parts = split(line, ' ');
        if(parts.size() > 0)
        {
            if(parts[0] == "v")
            {
                float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;
                switch(parts.size())
                {
                case 5: w = std::stod(parts[4]);
                case 4: z = std::stod(parts[3]);
                case 3: y = std::stod(parts[2]);
                case 2: x = std::stod(parts[1]);
                    break;
                }

                auto pt = transform * glm::vec4(x, y, z, w);
                points.push_back(glm::vec3(pt.x, pt.y, pt.z));
            }
            else if(parts[0] == "f")
            {
                outTriangles.push_back(Tri{ 
                    points[std::stoi(parts[1])], 
                    points[std::stoi(parts[2])], 
                    points[std::stoi(parts[3])] 
                });
            }
        }
    }
}
