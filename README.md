# GI Raytracer Project

## Requirements
 * A C++11 compliant compiler (e.g. **gcc** or **clang**)
 * GLM Library (currently in **include/glm**)
    
## To Setup
    mkdir build
    cd build
    cmake ..
    make
    cp ../config.example.xml config.xml
    
## To Run
The raytracer program requires an argument [config_name] which specifies the configuration xml file. The current output should be the spew of a tree structure which represents the XML's data.

    ./raytracer [config_name]

## To Format Code
Code formatting is always a pain. Use of clang-format can help code be more readable and more sane.

First install clang-format-3.5. Then to do formatting simply run the following code from the src directory:

    find -type f | xargs clang-format-3.5 -i -style="{BasedOnStyle: Mozilla, IndentWidth: 4}"

## Valid Configuration Options

### general
* **verbose (bool)** - Controls how much information is printed from the raytracer.

### pinhole
* **pixel_size (float)** - Not yet used
* **num_samples (int)** - Not yet used
* **eye (glm::vec3)** - The position of the camera
* **up (glm::vec3)** - The vector which defines "up" from the camera's perspective.
* **look_at (glm::vec)** - A vector along the line-of-sight of the camera.
* **view_distance (float)** - The distance from the pinhole to the viewplane.

### ppm_viewplane
* **hres (int)** - The number of pixels wide.
* **vres (int)** - The number of pixels high
* **filename (string)** - The name of the file to write the image to.

## naive_index
None required

## kdtree_index
Not yet implemented

## ambient
* **scale_radiance (float)** - Not yet used

## directional
* **direction (glm::vec3)** - Not yet used
* **scale_radiance (float)** - Not yet used

## color
* **name (stirng)** - The unique name of this color.
* **value (glm::vec3)** - The RGB values of this color.

## matte
* **name (string)** - The unique name of this material
* **color (string)** - The name of the color
* **ka (float)** - The contribution from ambient lighting
* **kd (float)** - The contribution from diffuse lighting

## sphere
* **point (glm::vec3)** - center of sphere
* **radius (float)** - self explainatory
* **material (string)** - the material which handles the coloration of the sphere

## plane
* **point (glm::vec3)** - point on plane
* **normal (glm::vec3)** - normal vector of plane
* **material (string)** - the material which handles the coloration of the plane

## rect
* **point (glm::vec3)** - point on plane
* **normal (glm::vec3)** - normal vector of plane
* **width (float)** - width of rectangle
* **height (float)** - height of rectangle
* **material (string)** - the material which handles the coloration of the plane
