
#include "kernel.hpp"

#include "util/xml_helpers.hpp"

#include <iostream>

using namespace raytracer;

int main(int argc, char* argv[])
{
    // Load the user settings into a kernel.
    Kernel kernel;
    loadXML("config.xml", kernel);

    std::cout << kernel.toString();

    return 0;
}

