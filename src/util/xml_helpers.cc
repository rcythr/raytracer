
#include "util/xml_helpers.hpp"
#include "util/split.hpp"

int raytracer::extractInt(ParamMap& params, const char* key, int default_value) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        return std::stoi(find_itr->second);
    }
    return default_value;
}

int raytracer::extractInt(ParamMap& params, const char* key) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        return std::stoi(find_itr->second);
    }
    throw std::runtime_error(std::string("Missing attribute ") + key);
}

float raytracer::extractFloat(ParamMap& params, const char* key, float default_value) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        return std::stof(find_itr->second);
    }
    return default_value;
}

float raytracer::extractFloat(ParamMap& params, const char* key) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        return std::stof(find_itr->second);
    }
    throw std::runtime_error(std::string("Missing attribute ") + key);
}

double raytracer::extractDouble(ParamMap& params, const char* key, double default_value) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        return std::stod(find_itr->second);
    }
    return default_value;
}

double raytracer::extractDouble(ParamMap& params, const char* key) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        return std::stod(find_itr->second);
    }
    throw std::runtime_error(std::string("Missing attribute ") + key);
}

std::string raytracer::extractString(ParamMap& params, const char* key, std::string default_value) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        return find_itr->second;
    }
    return default_value;
}

std::string raytracer::extractString(ParamMap& params, const char* key) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        return find_itr->second;
    }
    throw std::runtime_error(std::string("Missing attribute ") + key);
}

bool raytracer::extractBool(ParamMap& params, const char* key, bool default_value) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        std::string& val = find_itr->second;
        if (val == "1" || val == "true" || val == "yes" || val == "on") {
            return true;
        }
        return false;
    }
    return default_value;
}

bool raytracer::extractBool(ParamMap& params, const char* key) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        std::string& val = find_itr->second;
        if (val == "1" || val == "true" || val == "yes" || val == "on") {
            return true;
        }
        return false;
    }
    throw std::runtime_error(std::string("Missing attribute ") + key);
}

glm::vec3 raytracer::extractVec3(ParamMap& params, const char* key, glm::vec3 default_value) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        auto val = split(find_itr->second, ' ');
        if (val.size() == 3) {
            return glm::vec3(std::stof(val[0]), std::stof(val[1]),
                             std::stof(val[2]));
        }
    }
    return default_value;
}

glm::vec3 raytracer::extractVec3(ParamMap& params, const char* key) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        auto val = split(find_itr->second, ' ');
        if (val.size() == 3) {
            return glm::vec3(std::stof(val[0]), std::stof(val[1]),
                             std::stof(val[2]));
        }
    }
    throw std::runtime_error(std::string("Missing attribute ") + key);
}

glm::vec2 raytracer::extractVec2(ParamMap& params, const char* key, glm::vec2 default_value) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        auto val = split(find_itr->second, ' ');
        if (val.size() == 2) {
            return glm::vec2(std::stof(val[0]), std::stof(val[1]));
        }
    }
    return default_value;
}

glm::vec2 raytracer::extractVec2(ParamMap& params, const char* key) {
    auto find_itr = params.find(key);
    if (find_itr != params.end()) {
        auto val = split(find_itr->second, ' ');
        if (val.size() == 2) {
            return glm::vec2(std::stof(val[0]), std::stof(val[1]));
        }
    }
    throw std::runtime_error(std::string("Missing attribute ") + key);
}
