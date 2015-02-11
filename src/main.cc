
#include "kernel.hpp"
#include "util/xml_helpers.hpp"
#include <iostream>

#include "util/ppm/ppm.hpp"

using namespace raytracer;

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "Arguments: raytracer [config_filename]" << std::endl;
        return EXIT_FAILURE;
    }

    // Load the user settings into a kernel.
    Kernel kernel;
    loadXML(argv[1], kernel);

    if(kernel.verbose)
    {
        std::cout << kernel.toString();
    }

    kernel.camera->render(kernel.spatial_index, 
                          kernel.ambient_light, 
                          kernel.lights);

    return EXIT_SUCCESS;
}

