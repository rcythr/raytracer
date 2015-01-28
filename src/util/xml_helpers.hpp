// Copyright (c) 2013 Richard Laughlin
// All Rights Reserved
#pragma once

#include "util/xml.hpp"
#include <fstream>

#include <glm/glm.hpp>

namespace raytracer {

int extractInt(ParamMap& params, const char* key, int default_value);
float extractFloat(ParamMap& params, const char* key, float default_value);
double extractDouble(ParamMap& params, const char* key, double default_value);
std::string extractString(ParamMap& params, const char* key,
                          std::string default_value);
bool extractBool(ParamMap& params, const char* key, bool default_value);
glm::vec3 extractVec3(ParamMap& params, const char* key,
                      glm::vec3 default_value);

template <typename T>
void loadXML(const std::string filename, T& handler) {
    std::ifstream input(filename);
    if (input.good()) {
        std::string data{std::istreambuf_iterator<char>(input),
                         std::istreambuf_iterator<char>()};
        parseXml(data, handler);
    } else {
        throw std::runtime_error(filename + " is missing!!");
    }
}
}
