#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <vector>
#include <algorithm>
#include <iostream>

#include "Entity.hpp"
#include "System.hpp"

class EntityManager {
public:
    Entity* create_entity() {
        Entity* entity = new Entity(m_NextEntityId++);
        m_Entities.push_back(entity);
        return entity;
    }

    void destroy_entity(Entity* entity) {
        m_Entities.erase(std::remove(m_Entities.begin(), m_Entities.end(), entity), m_Entities.end());
        delete entity;
    }

    void add_system(System* system) {
        m_Systems.push_back(system);
    }

    void remove_system(System* system) {
        m_Systems.erase(std::remove(m_Systems.begin(), m_Systems.end(), system), m_Systems.end());
        delete system;
    }

    void start() {
        for (System* system : m_Systems) {
            system->on_start();
        }
    }

    void update(float deltaTime) {
        for (System* system : m_Systems) {
            system->on_update(deltaTime);
        }
    }

private:
    int m_NextEntityId = 0;
    std::vector<Entity*> m_Entities;
    std::vector<System*> m_Systems;
};

#endif