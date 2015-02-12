
#include "camera/pinhole.hpp"

#include "spatial/spatial_index.hpp"
#include "light/light.hpp"
#include "view_plane/view_plane.hpp"

#include "util/string_mult.hpp"

#include <sstream>

using namespace raytracer;

void PinholeCamera::spawn_rays(std::function<void(size_t, size_t, Ray&)> spawn_callback)
{
}

std::string PinholeCamera::toString(size_t depth) 
{
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;
    ss << tabdepth << "Type: PINHOLE\n";
    ss << tabdepth << "HRES: " << view_plane->get_width() << '\n';
    ss << tabdepth << "VRES: " << view_plane->get_height() << '\n';
    ss << tabdepth << "PIXEL SIZE: " << pixel_size << '\n';
    ss << tabdepth << "NUM SAMPLES: " << num_samples << '\n';
    ss << tabdepth << "LOCATION: (" << point.x << ',' << point.y << ',' << point.z << ")\n";
    ss << tabdepth << "UP: (" << up.x << ',' << up.y << ',' << up.z << ")\n";
    ss << tabdepth << "LOOK AT: (" << look_at.x << ',' << look_at.y << ',' << look_at.z << ")\n";
    ss << tabdepth << "VIEW DISTANCE: " << view_distance << '\n';
    return ss.str();
}
