#include "shape/sphere.hpp"

#include "util/vec3_helpers.hpp"
#include "util/string_mult.hpp"

#include <iostream>
#include <sstream>
#include <math.h> 

using namespace raytracer;

bool Sphere::test_hit(Ray& ray, double& tmin)
{

	//Major values for texting interstion
	//a, b, and c are part of the intersection equation
	glm::vec3 oc;

	oc = ray.origin-point;

	//b=2(l*(o-c))
	auto b = glm::dot(ray.direction,oc)*2.0f;
	//c = (o-c)^2-r^2
	auto c = dot(oc,oc)- radius*radius;
	//testVal = b^2-c;
	auto testVal = b*b - c*4.0f;
	//float x, y, z;
	//x = testVec.x; y = testVec.y; z=testVec.z;
	//float testVal = sqrt(x*x+y*y+z*z);

	//if b^2-c >= 0
	if(testVal == 0.0){
		//First intersection
		tmin = -b/2;
		return true;
	} else if(testVal > 0.0) {
		float t1, t2;
		t1 = (-b/2) + sqrt(testVal);
		t2 = (-b/2) - sqrt(testVal);
		tmin = std::min(t1, t2);
		return true;
	}
	else{
    	return false;
    }
}

std::string Sphere::toString(size_t depth)
{
    std::string tabdepth = std::string("\t") * depth;

    std::stringstream ss;

    ss << tabdepth << "TYPE: SPHERE\n";
    ss << tabdepth << "LOCATION: " << point << '\n';
    ss << tabdepth << "RADIUS: " << radius << '\n';
    ss << tabdepth << "MATERIAL: \n";
    ss << material->toString(depth+1);

    return ss.str();
}
