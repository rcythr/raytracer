
#include "kernel.hpp"
#include "util/xml_helpers.hpp"
#include <iostream>

#include "util/ppm/ppm.hpp"

using namespace raytracer;

int main(int argc, char* argv[])
{
    std::string config_name = "config.xml";

    if(argc == 2)
        config_name = argv[1];

    // Load the user settings into a kernel.
    Kernel kernel;
    loadXML(config_name, kernel);

    std::cout << kernel.toString();

    /*
    PPM test(800, 600);

    for(size_t i=0; i < 800; ++i)
    {
        test.set_pixel(300, i, glm::vec3(1.0, 0.0, 0.0));
    }

    test.save("out.ppm");
    */

    return 0;
}

