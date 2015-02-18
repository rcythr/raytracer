#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace raytracer {

struct ViewPlane {
    virtual ~ViewPlane() {}

    /**
     * Returns the width (in pixels) of the viewing plane
     */
    virtual size_t get_width() = 0;

    /**
     * Returns the height (in pixels) of the viewing plane
     */
    virtual size_t get_height() = 0;

    /**
     * Returns the value of the specified pixel in r, g, b.
     *   Each value is measured from 0.0 -> 1.0
     */
    virtual glm::vec3 get_pixel(size_t row, size_t col) = 0;

    /**
     * Sets the value of the specified pixel in r, g, b.
     *   Each value is measured from 0.0 -> 1.0
     */
    virtual void set_pixel(size_t row, size_t col, glm::vec3 color) = 0;

    /**
     * Performs an unspecified operation which can be used to store
     *   the result of the computation at this point in time.
     * Ex. The PPMViewPlane writes out the image file when this is called.
     */
    virtual void finish() = 0;
};

typedef std::shared_ptr<ViewPlane> ViewPlanePtr;
}
