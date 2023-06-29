#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Model.hpp"
#include "Shader.hpp"
#include "Keywords.hpp"

class Window {
public:
    Window(const char* title = "untitled", const int width = -1, const int height = -1, bool fullscreen = false)
     : m_Title(title), m_Fullscreen(fullscreen)
    {
        m_MonitorWidth = width;
        m_MonitorHeight = height;

        if (!glfwInit())
            std::cerr << "Error: GLFW Init Fialed.\n";

        m_Monitor = glfwGetPrimaryMonitor();
        if (!m_Monitor) {
            glfwTerminate();
            std::cerr << "Error: Monitor Not Found.\n";
        }

        if (width <= 0 || height <= 0 || m_Fullscreen)
            glfwGetMonitorWorkarea(m_Monitor, NULL, NULL, &m_MonitorWidth, &m_MonitorHeight);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        if (m_Fullscreen)
            m_Window = glfwCreateWindow(m_MonitorWidth, m_MonitorHeight, m_Title, m_Monitor, NULL);
        else
            m_Window = glfwCreateWindow(m_MonitorWidth, m_MonitorHeight, m_Title, NULL, NULL);

        if (!m_Window) {
            glfwTerminate();
            std::cerr << "Error: Window Not Found.\n";
        }

        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1);

        if (glewInit() != GLEW_OK)
            std::cerr << "Error: GLEW Init Failed.\n";

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            std::cerr << "OpenGL Error: " << err << std::endl;

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_Renderer = std::make_unique<Renderer>();
    }

    ~Window() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void attach_shader(Shader shader) {
        m_Shader = std::make_unique<Shader>(shader);
        m_Shader->bind();
    }

    void draw(Model model) {

    }

    GLFWwindow* get_glfw_window() { return m_Window; }
    GLFWmonitor* get_glfw_monitor() { return m_Monitor; }

private:
    GLFWwindow* m_Window;
    GLFWmonitor* m_Monitor;
    const char* m_Title;
    int m_MonitorWidth, m_MonitorHeight;
    bool m_Fullscreen;
    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<Shader> m_Shader;
};

#endif