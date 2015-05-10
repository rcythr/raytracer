#include "photon_map.hpp"

using namespace raytracer;

void raytracer::insert_photon(RawPhotonMap& map, std::shared_ptr<Shape> ptr, Photon& p)
{
    auto find_itr = map.find(ptr);
    if(find_itr != map.end())
    {
        find_itr->second.push_back(p);
    }
    else
    {
        std::vector<Photon> ps;
        ps.push_back(p);
        map.insert(std::make_pair(ptr, ps));
    }
}
