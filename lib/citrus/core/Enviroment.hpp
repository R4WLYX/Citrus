#ifndef ENVIROMENT_HPP
#define ENVIROMENT_HPP

#define InvalidEntity nullptr

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <thread>
#include <chrono>

#include "Entity.hpp"
#include "Component.hpp"
#include "Window.hpp"
#include "Renderer.hpp"

template<typename C>
struct With {

};

class Enviroment {
private:
    typedef std::chrono::high_resolution_clock Clock;

    typedef void (*StartSystem)(Enviroment*);
    typedef void (*UpdateSystem)(Enviroment*, float);

    Window m_Window;
    Renderer m_Renderer;

    std::vector<void*> Entities;
    
    std::vector<StartSystem> start_systems;
    std::vector<UpdateSystem> update_systems;

public:
    Enviroment(const Window& window, const Renderer& renderer)
     : m_Window(window), m_Renderer(renderer)
    {}

    void run() {
        for (auto& system : start_systems)
            system(this);

        auto T0 = Clock::now();
        std::chrono::duration<float> deltaTime;

        while (!glfwWindowShouldClose(m_Window.get_glfw_window())) {
            auto T1 = Clock::now();
            deltaTime = T1 - T0;
            m_Renderer.clear();

            for (auto& system : update_systems)
                system(this, deltaTime.count());
            
            glfwSwapBuffers(m_Window.get_glfw_window());
            T0 = T1;
        }
    }

    Window* window() { return &m_Window; }
    Renderer* renderer() { return &m_Renderer; }

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
    std::vector<E*> query() {
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
    std::vector<C*> query() {
        const char
            *entityType = typeid(E).name(),
            *componentType = typeid(C).name();
        std::vector<C*> queriedComponents;
        E* entity;
        C* component;

        for (int i{}; i < Entities.size(); i++) {
            entity = static_cast<E*>(Entities[i]);
            component = entity->template get_component<C>();
            if (component->type == componentType/* && entity->type == entityType*/)
                queriedComponents.push_back(component);
        }
        
        return queriedComponents;
    }
};

#endif