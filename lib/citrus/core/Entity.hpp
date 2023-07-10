#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>
#include <unordered_map>

struct Entity {
private:
    std::unordered_map<const std::type_info*, void*> components;

public:
    template <typename C>
    Entity* add_component(C component) {
        static_assert(std::is_copy_constructible_v<C> || std::is_move_constructible_v<C>, "Component must be copyable or movable.");
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