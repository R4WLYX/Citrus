#ifndef ENTITY_HPP
#define ENTITY_HPP

#define InvalidComponent nullptr

#include <vector>

#include "Component.hpp"

template <typename E>
struct Entity {
private:
    std::vector<void*> Components;

public:
    const char* type;

    Entity()
    : type(typeid(E).name())
    {}

    template <typename C>
    Entity<E>* add_component() {
        if (get_component<C>() == InvalidComponent)
            Components.push_back(new C());
        return this;
    }

    template <typename C>
    Entity<E>* add_component(C& component) {
        if (get_component<C>() == InvalidComponent)
            Components.push_back(&component);
        return this;
    }

    template <typename C>
    Entity<E>* add_component(C&& component) {
        if (get_component<C>() == InvalidComponent)
            Components.push_back(&component);
        return this;
    }

    template <typename C>
    C* get_component() const {
        const char* componentType = typeid(C).name();
        C* component;

        for (int i{}; i < Components.size(); i++) {
            component = static_cast<C*>(Components[i]);
            if (component->type == componentType)
                return component;
        }

        return InvalidComponent;
    }
};

#endif