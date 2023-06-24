#ifndef ENVIROMENT_HPP
#define ENVIROMENT_HPP

#define InvalidEntity nullptr

#include <vector>
#include <thread>
#include <chrono>

#include "Entity.hpp"
#include "Component.hpp"

template<typename C>
struct With {

};

class Enviroment {
private:
    typedef std::chrono::high_resolution_clock Clock;

    typedef void (*StartSystem)(Enviroment*);
    typedef void (*UpdateSystem)(Enviroment*, float);

    std::vector<void*> Entities;
    
    std::vector<StartSystem> start_systems;
    std::vector<UpdateSystem> update_systems;

    bool running;

public:
    Enviroment()
     : running(false)
    {}

    ~Enviroment() {
        running = false;
    }

    void run() {
        for (auto& system : start_systems)
            system(this);

        auto T0 = Clock::now();
        std::chrono::duration<float> deltaTime;

        running = true;

        while (running) {
            auto T1 = Clock::now();
            deltaTime = T1 - T0;

            for (auto& system : update_systems)
                system(this, deltaTime.count());
            
            T0 = T1;
        }
    }

    template <typename E>
    Enviroment* create_entity() {
        Entities.push_back(new E());
        return this;
    }

    template <typename E>
    Enviroment* add_entity(E& entity) {
        Entities.push_back(&entity);
        return this;
    }

    template <typename E>
    Enviroment* add_entity(E&& entity) {
        Entities.push_back(&entity);
        return this;
    }

    Enviroment* add_system(StartSystem system) {
        start_systems.push_back(system);
        return this;
    }

    Enviroment* add_system(UpdateSystem system) {
        update_systems.push_back(system);
        return this;
    }

    template <typename E>
    std::vector<E*> query() const {
        const char* entityType = typeid(E).name();
        std::vector<E*> queriedEntities;
        E* entity;

        for (int i{}; i < Entities.size(); i++) {
            entity = static_cast<E*>(Entities[i]);
            if (entity->type == entityType)
                queriedEntities.push_back(entity);
        }
        
        return queriedEntities;
    }

    // template <typename E, typename C, typename With<C>>
    // std::vector<E*> query() const {
    //     const char* entityType = typeid(E).name();
    //     std::vector<E*> queriedEntities;
    //     E* entity;

    //     for (int i{}; i < Entities.size(); i++) {
    //         entity = static_cast<E*>(Entities[i]);
    //         if (entity->type == entityType)
    //             queriedEntities.push_back(entity);
    //     }
        
    //     return queriedEntities;
    // }

    template <typename E, typename C>
    std::vector<C*> query() const {
        const char* componentType = typeid(C).name();
        std::vector<C*> queriedComponents;
        C* component;

        for (int i{}; i < Entities.size(); i++) {
            component = static_cast<E*>(Entities[i])->template get_component<C>();
            if (component->type == componentType)
                queriedComponents.push_back(component);
        }
        
        return queriedComponents;
    }
};

#endif