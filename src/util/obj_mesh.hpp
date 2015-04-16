#pragma once

#include "material/material.hpp"
#include "spatial/spatial_index.hpp"

#include <string>
#include <glm/glm.hpp>

namespace raytracer {

void loadObj(std::string filename, glm::vec3 point, glm::vec3 rotate,
             float scale, MaterialPtr material, SpatialIndexPtr index);
}
