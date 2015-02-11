
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

    /*
    PPM test(800, 600);

    for(size_t i=0; i < 800; ++i)
    {
        test.set_pixel(300, i, glm::vec3(1.0, 0.0, 0.0));
    }

    test.save("out.ppm");
    */

    return EXIT_SUCCESS;
}

