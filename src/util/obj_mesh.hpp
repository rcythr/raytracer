#pragma once

#include "material/material.hpp"
#include "spatial/spatial_index.hpp"

#include <string>
#include <glm/glm.hpp>

namespace raytracer {

void loadObj(std::string filename, glm::mat4 transform, MaterialPtr material,
             SpatialIndexPtr index);
}
