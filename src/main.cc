
#include "kernel.hpp"
#include "util/xml_helpers.hpp"
#include <iostream>
#include <chrono>

#include "util/ppm/ppm.hpp"

using namespace raytracer;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Arguments: raytracer [config_filename]" << std::endl;
        return EXIT_FAILURE;
    }

    // Load the user settings into a kernel.
    Kernel kernel;
    loadXML(argv[1], kernel);

    typedef std::chrono::high_resolution_clock Clock;
    auto start = Clock::now();
    kernel.spatial_index->optimize();
    auto end = Clock::now();
    std::cout << "KD-Tree Build Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     end - start).count() << " miliseconds" << std::endl;

    kernel.render();
    return EXIT_SUCCESS;
}
