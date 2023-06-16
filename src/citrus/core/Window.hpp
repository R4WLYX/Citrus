#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "System.hpp"

#include "Renderer.hpp"

typedef std::chrono::high_resolution_clock m_Clock;
typedef std::chrono::high_resolution_clock::time_point m_Time;

class Window {
public:
    Window(const char* title = "default", int width = 100, int height = 100) {
        if (!glfwInit())
            std::cerr << "Error\n";

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(width, height, title, NULL, NULL);

        if (!m_Window)
            glfwTerminate();

        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1);

        if (glewInit() != GLEW_OK)
            std::cerr << "Error\n";

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }

    ~Window() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    EntityManager* get_manager() {
        return &m_Manager;
    }

    void start() {
        m_Manager.start();

        while (!glfwWindowShouldClose(m_Window))
            update();
    }

    void update() {
        T1 = m_Clock::now();
        std::chrono::duration<float> deltaTime = T1 - T0;
        glfwPollEvents();
        m_Renderer.clear();
        
        m_Manager.update(deltaTime.count());

        glfwSwapBuffers(m_Window);
        T0 = T1;
    }

    void stop() {
        glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
    }

private:
    GLFWwindow* m_Window;
    EntityManager m_Manager;
    Renderer m_Renderer;
    m_Time T0, T1;
};

#endif