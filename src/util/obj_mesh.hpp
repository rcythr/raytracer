#pragma once

#include "material/material.hpp"
#include "spatial/spatial_index.hpp"

#include <string>
#include <glm/glm.hpp>

namespace raytracer {

void loadObj(std::string filename, MaterialPtr material, SpatialIndexPtr index);
}
