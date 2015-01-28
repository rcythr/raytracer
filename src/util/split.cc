#include "util/split.hpp"

std::vector<std::string> raytracer::split(std::string str, char sep)
{
    std::vector<std::string> result;

    std::string buf;
    size_t len = str.size();
    for(size_t i=0; i < len; ++i)
    {
        if(str[i] == sep)
        {
            result.push_back(buf);
            buf.clear();
        }
        else
        {
            buf.push_back(str[i]);
        }
    }

    if(buf.size() != 0)
    {
        result.push_back(std::move(buf));
    }

    return std::move(result);
}
