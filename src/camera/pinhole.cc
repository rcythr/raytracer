
#include "camera/pinhole.hpp"

#include "spatial/spatial_index.hpp"
#include "light/light.hpp"
#include "view_plane/view_plane.hpp"

#include "util/string_mult.hpp"

#include <glm/gtc/matrix_inverse.hpp>
#include <random>
#include <sstream>

using namespace raytracer;

glm::mat4 PinholeCamera::build_transform_mat() {
    glm::vec3 zaxis = glm::normalize(point - look_at);
    glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
    glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));

    // Please note this is given to mat4 as COLUMN vectors (so it appears
    // transposed!)
    glm::mat4 result = { glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0),
                         glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0),
                         glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0),
                         glm::vec4(-glm::dot(xaxis, point),
                                   -glm::dot(yaxis, point),
                                   -glm::dot(zaxis, point), 1) };

    return result;
}

void PinholeCamera::spawn_rays( std::function<void(size_t, size_t, Ray&)> spawn_callback) {
    auto inverseTransform = glm::inverse(build_transform_mat());

    size_t num_rows = view_plane->get_height();
    size_t num_cols = view_plane->get_width();

    float half_width = 0.5 * pixel_size * num_cols;
    float half_height = 0.5 * pixel_size * num_rows;

    float sub_pixel_size = pixel_size / (1 + num_samples );

    // First calculate the location of the top left pixel
    glm::vec4 pixelPt(-half_width, half_height, -view_distance, 0.0f);

    // Build the ray located at the pinhole.
    Ray r;
    r.origin = glm::vec3(inverseTransform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    for (size_t row = 0; row < num_rows; ++row) {
        pixelPt.y -= sub_pixel_size;
        for (size_t srow = 0; srow < num_samples; ++srow) {
            pixelPt.x = -half_width;
            for(size_t col = 0; col < num_cols; ++ col) {
                pixelPt.x += sub_pixel_size;
                for(size_t scol = 0; scol < num_samples; ++scol) {
                    r.direction = glm::vec3(glm::normalize(inverseTransform * pixelPt));
                    r.update();
                    spawn_callback(row, col, r);
                    pixelPt.x += sub_pixel_size;
                }
            }
            pixelPt.y -= sub_pixel_size;
        }
    }
}

std::string PinholeCamera::toString(size_t depth) {
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;
    ss << tabdepth << "Type: PINHOLE\n";
    ss << tabdepth << "HRES: " << view_plane->get_width() << '\n';
    ss << tabdepth << "VRES: " << view_plane->get_height() << '\n';
    ss << tabdepth << "PIXEL SIZE: " << pixel_size << '\n';
    ss << tabdepth << "NUM SAMPLES: " << num_samples << '\n';
    ss << tabdepth << "LOCATION: (" << point.x << ',' << point.y << ','
       << point.z << ")\n";
    ss << tabdepth << "UP: (" << up.x << ',' << up.y << ',' << up.z << ")\n";
    ss << tabdepth << "LOOK AT: (" << look_at.x << ',' << look_at.y << ','
       << look_at.z << ")\n";
    ss << tabdepth << "VIEW DISTANCE: " << view_distance << '\n';
    return ss.str();
}
