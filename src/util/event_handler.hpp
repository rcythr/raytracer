#pragma once

#include <unordered_map>

namespace raytracer
{

template<typename EventTy>
struct EventHandler
{
    typedef std::function<void(void*)> GenericEventFunc;

    template<typename T>
    using EventFunc = std::function<void(T*)>;

    template<typename T>
    void register_event(EventTy event_id, EventFunc<T> handler)
    {
        handlers.emplace(event_id, [=](void* event) {
            handler(static_cast<T*>(event));
        });
    }

    template<typename T>
    void fire(EventTy event_id, T* ptr) 
    {
        auto find_itr = handlers.find(event_id);
        if(find_itr != handlers.end())
        {
            find_itr->second(ptr);     
        }
        delete ptr;
    }

private:
    std::unordered_map<EventTy, GenericEventFunc> handlers;
};

}
