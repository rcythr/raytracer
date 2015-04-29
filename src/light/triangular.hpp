#pragma once

#include "light/light.hpp"
#include "shape/triangle.hpp"

namespace raytracer {
    struct TriangularLight : public Light {
        TriangularLight(float intensity, std::shared_ptr<Triangle> triangle, glm::vec3 color)
            : Light(intensity, color)
            , triangle(triangle)
        {}

        virtual std::string toString(size_t depth = 0) override;

        virtual LightType get_type() override { return LightType::TRIANGULAR; }

		virtual std::vector<Ray> get_photons(int n){
				//Create Ray vector
				vector<Ray> rays;

				//Create random device for uniform distribution
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<> dis(0, 1);

				for(int i = 0; i < n; i++){
					//Create ray
					Ray r;
					//Generate origin
					float r1 = dis(gen), r2 = dis(gen);
					r->origin = (1-sqrt(r1)) * triangle->p0 +
								(sqrt(r1)*(1-r2)) * triangle->p1 +
								(sqrt(r1) * r2) * triangle->p2;
					//Generate direction
					r1 = dis(gen), r2 = dis(gen);
					glm::vec3 randPoint = (1-sqrt(r1)) * triangle->p0 +
								(sqrt(r1)*(1-r2)) * triangle->p1 +
								(sqrt(r1) * r2) * triangle->p2;
					//Generate normal
					glm::vec3 normal = glm::normalize(glm::cross((triangle->p1-triangle->p0),
																(triangle->p2-triangle-p0));
					glm::vec3 direction = randPoint + normal;

					//Set the direction
					r.direction = direction - r.origin;

					//Update and push ray
					r.update();
					rays.push_back(r);
				}
		};

        std::shared_ptr<Triangle> triangle;
    };
}
