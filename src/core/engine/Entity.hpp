#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>

#include "Component.h"

class Entity {
public:
    Entity(int id) : m_ID(id) {}

    void add_component(Component* component) {
        m_Components.push_back(component);
    }

    void remove_component(Component* component) {
        m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), component), m_Components.end());
    }

    template <typename T>
    T* get_component() const {
        for (Component* component : m_Components) {
            if (T* castedComponent = dynamic_cast<T*>(component)) {
                return castedComponent;
            }
        }
        return nullptr;
    }

private:
    int m_ID;
    std::vector<Component*> m_Components;
};

#endif