#pragma once

#include "camera/camera.hpp"
#include "camera/pinhole.hpp"

#include "light/light.hpp"
#include "light/ambient.hpp"
#include "light/directional.hpp"
#include "light/point.hpp"

#include "material/material.hpp"
#include "material/phong.hpp"

#include "sampler/sampler.hpp"
#include "sampler/solid.hpp"
#include "sampler/checkerboard.hpp"
#include "sampler/rainbow.hpp"
#include "sampler/circles.hpp"

#include "shape/shape.hpp"
#include "shape/sphere.hpp"
#include "shape/triangle.hpp"

#include "spatial/spatial_index.hpp"
#include "spatial/naive.hpp"
#include "spatial/kdtree.hpp"

#include "view_plane/view_plane.hpp"
#include "view_plane/ppm.hpp"

#include "tracers.hpp"
