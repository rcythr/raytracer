#pragma once

#include "camera/camera.hpp"
#include "camera/pinhole.hpp"

#include "light/light.hpp"
#include "light/ambient.hpp"
#include "light/directional.hpp"
#include "light/point.hpp"
#include "light/triangular.hpp"
#include "light/spherical.hpp"

#include "material/material.hpp"
#include "material/phong.hpp"
#include "material/lambertian.hpp"
#include "material/photon_mapped.hpp"

#include "sampler/sampler.hpp"
#include "sampler/solid.hpp"
#include "sampler/checkerboard.hpp"
#include "sampler/rainbow.hpp"
#include "sampler/circles.hpp"
#include "sampler/png_texture.hpp"

#include "shape/shape.hpp"
#include "shape/sphere.hpp"
#include "shape/triangle.hpp"

#include "spatial/spatial_index.hpp"
#include "spatial/kdtree.hpp"
#include "spatial/naive.hpp"

#include "view_plane/view_plane.hpp"
#include "view_plane/ppm.hpp"

#include "tone_operators/tone_operator.hpp"
#include "tone_operators/gaussian_operator.hpp"
#include "tone_operators/ward_operator.hpp"
#include "tone_operators/reinhard_operator.hpp"
#include "tone_operators/linear_operator.hpp"
