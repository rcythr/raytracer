
#include "util/obj_mesh.hpp"
#include "util/split.hpp"

#include "shape/triangle.hpp"

#include <fstream>

void raytracer::loadObj(std::string filename, MaterialPtr material,
                        SpatialIndexPtr index) {
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
                points.push_back(glm::vec3(x / w, y / w, z / w));
            } else if (parts[0] == "f") {
                index->insert(std::make_shared<Triangle>(
                    points[std::stoi(parts[1])], points[std::stoi(parts[2])],
                    points[std::stoi(parts[3])], material));
            }
        }
    }
}
