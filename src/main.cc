
#include "kernel.hpp"
#include "util/xml_helpers.hpp"
#include <iostream>
#include <chrono>

#include "util/ppm/ppm.hpp"

using namespace raytracer;

int main(int argc, char* argv[]) {

    auto triangle = std::make_shared<Triangle>(
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f),
        nullptr);

    auto light = std::make_shared<TriangularLight>(1000.f, triangle, glm::vec3(0.0f));

    auto rays = light->get_photons(100000);
    for(auto& ray : rays)
    {
        std::cout << "Origin: " << ray.origin << " | Direction: " << ray.direction << std::endl;
    }

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
