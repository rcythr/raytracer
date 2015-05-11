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

### color
* **name (stirng)** - The unique name of this color.
* **value (glm::vec3)** - The RGB values of this color.

### general
* **background_color (vec3)** - The background color of the scene. Used when no other object is hit by a ray.
* **num_threads (int)** - The number of threads that execute concurrently to render the scene.
* **world_ki (float)** - Specifies the index of refraction of the "air" surrounding all objects in the scene.
* **num_photons (int)** - For photon-mapped scenes, specifies the number of photons to simulate from the light sources.
* **final_gather_samples (int)** - For photon-mapped scenes, specifies the number of indirect lighting samples to take.
* **global_knn (int)** - For photon-mapped scenes, specifies the number of nearest neighbors to consider when sampling indirect light.
* **global_exposure (float)** For photon-mapped scenes, specifies an amplification of the indirect light to aid in visibility.
* **caustic_knn (int)** - For photon-mapped scenes, specifies the number of nearest neighbors to consider when sampling caustics.
* **caustic_exposure (float)** - For photon-mapped scenes, specifics an amplication of the caustic light to aid in visibility.

### pinhole
* **pixel_size (float)** - The size, in world units, of each pixel.
* **num_samples (int)** - The number of samples to be taken per direction, per pixel. 
* **eye (glm::vec3)** - The position of the camera, in world units.
* **up (glm::vec3)** - The vector which defines "up" from the camera's perspective, in world units.
* **look_at (glm::vec)** - A vector along the line-of-sight of the camera, in world units.
* **view_distance (float)** - The distance from the pinhole to the viewplane, in world units.

### ppm_viewplane
* **hres (int)** - The number of pixels wide.
* **vres (int)** - The number of pixels high
* **filename (string)** - The name of the file to write the image to.

### naive_index
None required

### ambient
* **intensity (float)** - The power of the ambient light.
* **color (vec3)** - The color of the ambient light.

### directional
* **intensity (float)** - The power of the directional light.
* **direction (vec3)** - The direction of the directional light.
* **color (vec3)** - The color of the ambient light.

### triangular
* **p0 (vec3)** - The first point on the triangle of light.
* **p1 (vec3)** - The second point on the triangle of light.
* **p2 (vec3)** - The third point on the triangle of light.
* **intensity (float)** - The power of the triangular light.
* **color (vec3)** - The color of the triangular light.

### phong
* **ka (float)** - Ambient coefficient.
* **kd (float)** - Diffuse coefficient.
* **ks (float)** - Specular coefficient.
* **ke (float)** - Specular exponent
* **kt (float)** - Transmission coefficient.
* **ki (float)** - The index of refraction.
* **is_hollow (bool)** - true if the object is hollow, false otherwise.

### photon_mapped
* **kd (float)** - Diffuse coefficient.
* **ks (float)** - Specular coefficient.
* **kt (float)** - Transmission coefficient.
* **ki (float)** - The index of refraction.
* **is_hollow (bool)** - true if the object is hollow, false otherwise.

### solid
* **color (string)** - The solid color to use

### checkered
* **color1 (string)** - The first color of the checkerboard.
* **color2 (string)** - The second color of the checkerboard.

### circles
* **color1 (string)** - The first color of the circle checkerboard.
* **color2 (string)** - The second color of the circle checkerboard.

### rainbow
* **color1 (string)** - The first color of the interpolation
* **color2 (string)** - The second color of the interpolation

### png
* **filename (string)** - The filename of the png texture to use.

### sphere
* **point (vec3)** - center of sphere
* **radius (float)** - self explainatory
* **material (string)** - the material which handles the coloring of the sphere

### triangle
* **p0 (vec3)** - The first point, in world units.
* **u0 (vec2)** - The uv-coordinates of the first point.
* **p1 (vec3)** - The second point, in world units.
* **u1 (vec2)** - The uv-coordinates of the second point.
* **p2 (vec3)** - The third point, in world units.
* **u1 (vec2)** - The uv-coordinates of the third point.
* **material (string)** - the material which handles the coloring of the plane
