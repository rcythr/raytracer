#pragma once

#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>
#include "util/point_kdtree/point_kdtree.hpp"

namespace raytracer {

struct Shape;

struct Photon {
    glm::vec3 point;
    glm::vec3 incident_direction;
    glm::vec3 power;
    glm::vec3 color;

    bool operator!=(const Photon& other) const { return point != other.point; }
    bool operator==(const Photon& other) const { return point == other.point; }

    float& operator[](size_t i) { return point[i]; }
};

typedef std::unordered_map<std::shared_ptr<Shape>, std::vector<Photon>> RawPhotonMap;
typedef point_kdtree::PointKDTree<Photon, 3> PhotonMap;

PhotonMap optimizePhotonMap(RawPhotonMap& raw_map);

void insert_photon(RawPhotonMap& map, std::shared_ptr<Shape> ptr, Photon& p);

}
