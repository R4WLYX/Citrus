#ifndef ENTITY_HPP
#define ENTITY_HPP

#define InvalidComponent nullptr

#include <vector>
#include <unordered_map>

struct Entity {
private:
    std::unordered_map<const std::type_info*, void*> components;

public:
    template <typename C>
    Entity* add_component(C component) {
        components[&typeid(C)] = new C(component);
        return this;
    }

    template <typename C>
    C* get_component() {
        auto it = components.find(&typeid(C));
        if (it != components.end()) {
            return static_cast<C*>(it->second);
        }
        return nullptr;
    }
};

#endif