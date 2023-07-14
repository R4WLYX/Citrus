#ifndef ENVIROMENT_HPP
#define ENVIROMENT_HPP

#include "Entity.hpp"
#include "Window.hpp"
#include "InputHandler.hpp"
#include "Renderer.hpp"
#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Enviroment;

typedef void(*ON_STARTUP)(Enviroment*);
typedef void(*ON_UPDATE)(Enviroment*, double);
typedef void(*ON_PHYSICS)(Enviroment*, double, unsigned int);
typedef void(*ON_RENDER)(Enviroment*, Window*, const Renderer&);

class Enviroment {
private:
    typedef std::chrono::high_resolution_clock Clock;

    std::unique_ptr<Window> m_Window;
    std::unique_ptr<InputHandler> m_InputHandler;
    std::vector<ON_STARTUP> m_OnStartupSystems;
    std::vector<ON_PHYSICS> m_OnPhysicsUpdate;
    std::vector<ON_UPDATE> m_OnUpdate;
    std::vector<ON_RENDER> m_OnRender;
    std::vector<Entity*> Entities;
    unsigned int m_PhysicsSteps;

    Enviroment(const char* title = "untitled", int width = -1, int height = -1, bool fullscreen = false)
     : m_PhysicsSteps(8)
    {
        m_Window = std::make_unique<Window>(title, width, height, fullscreen);
        m_InputHandler = std::make_unique<InputHandler>(m_Window->get_glfw_window());
    }

public:
    static Enviroment* New(const char* title = "untitled", int width = -1, int height = -1, bool fullscreen = false)
    { return new Enviroment(title, width, height, fullscreen); }

    template<typename T>
    Enviroment* add_system(T system) {
        if      constexpr(std::is_same<T, ON_STARTUP>()) m_OnStartupSystems.push_back(system);
        else if constexpr(std::is_same<T, ON_PHYSICS>()) m_OnPhysicsUpdate.push_back(system);
        else if constexpr(std::is_same<T, ON_UPDATE>())  m_OnUpdate.push_back(system);
        else if constexpr(std::is_same<T, ON_RENDER>())  m_OnRender.push_back(system);

        return this;
    }

    template <typename E>
    Enviroment* create_entity() {
        Entities.push_back(new E());
        return this;
    }

    template <typename E>
    Enviroment* add_entity(E* entity) {
        Entities.push_back(entity);
        return this;
    }

    template <typename E, typename... Components>
    std::vector<E*> query() {
        std::vector<E*> queriedEntities;
        for (Entity* entity : Entities) {
            if (entity_has_components<E, Components...>(entity)) {
                queriedEntities.push_back(static_cast<E*>(entity));
            }
        }
        return queriedEntities;
    }

    template <typename E, typename C>
    bool entity_has_component(Entity* entity) {
        return entity->get_component<C>() != nullptr;
    }

    template <typename E, typename C, typename... Rest>
    bool entity_has_components(Entity* entity) {
        return entity_has_component<E, C>(entity) && entity_has_components<E, Rest...>(entity);
    }

    template <typename E>
    bool entity_has_components(Entity* entity) {
        return true;
    }

    Enviroment* set_physics_steps(unsigned int steps) {
        m_PhysicsSteps = steps;
        return this;
    }

    unsigned int get_physics_steps() { return m_PhysicsSteps; }

    int get_key_state(const char* key) {
        return m_InputHandler->get_key_map()[key];
    }

    Enviroment* run() {
        GLFWwindow* window = m_Window->get_glfw_window();
        Renderer renderer;

        auto T0 = Clock::now();
        std::chrono::duration<double> deltaTime;

        for (ON_STARTUP system : m_OnStartupSystems)
            system(this);

        glfwSwapInterval(1);

        while (!glfwWindowShouldClose(window)) {
            auto T1 = Clock::now();
            deltaTime = T1 - T0;
            
            renderer.clear();
            glfwPollEvents();

            if (m_OnPhysicsUpdate.size() != 0)
            for (ON_PHYSICS system : m_OnPhysicsUpdate) {
                const double subDeltaTime = deltaTime.count() / m_PhysicsSteps;
                for (int i{}; i < m_PhysicsSteps; i++)
                    system(this, subDeltaTime, m_PhysicsSteps);
            }
            
            if (m_OnUpdate.size() != 0)
            for (ON_UPDATE system : m_OnUpdate)
                system(this, deltaTime.count());

            if (m_OnRender.size() != 0)
            for (auto system : m_OnRender)
                system(this, m_Window.get(), renderer);

            m_InputHandler->update_key_map();
            
            glfwSwapBuffers(window);
            T0 = T1;
        }

        return this;
    }
};

#endif