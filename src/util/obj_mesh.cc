
#include "shape/triangle.hpp"

#include "util/obj_mesh.hpp"
#include "util/split.hpp"
#include "util/transforms.hpp"

#include <fstream>

void raytracer::loadObj(std::string filename, glm::vec3 point, glm::vec3 rotate,
                        float scale, MaterialPtr material,
                        SpatialIndexPtr index) {

    // First construct the matrix to transform things into world space.
    auto trans = buildTranslation(point) 
               * buildRotationZ(rotate.z) 
               * buildRotationY(rotate.y) 
               * buildRotationX(rotate.x) 
               * buildScale(scale);

    std::ifstream strm(filename.c_str());
    std::vector<glm::vec3> points;
    std::string line;

    while (std::getline(strm, line)) {
        auto parts = split(line, ' ');
        if (parts.size() > 0) {
            if (parts[0] == "v") {
                float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;
                switch (parts.size()) {
                    case 5:
                        w = std::stod(parts[4]);
                    case 4:
                        z = std::stod(parts[3]);
                    case 3:
                        y = std::stod(parts[2]);
                    case 2:
                        x = std::stod(parts[1]);
                        break;
                }
                auto pt = trans * glm::vec4(x, y, z, w);
                points.push_back(
                    glm::vec3(pt.x / pt.w, pt.y / pt.w, pt.z / pt.w));
            } else if (parts[0] == "f") {
                index->insert(std::make_shared<Triangle>(
                    points[std::stoi(parts[1]) - 1], glm::vec2(0.0f, 0.0f),
                    points[std::stoi(parts[2]) - 1], glm::vec2(0.0f, 1.0f),
                    points[std::stoi(parts[3]) - 1], glm::vec2(1.0f, 1.0f),
                    material));
            }
        }
    }
}
