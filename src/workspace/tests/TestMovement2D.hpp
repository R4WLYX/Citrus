#ifndef TEST_MOVEMENT_2D_HPP
#define TEST_MOVEMENT_2D_HPP

#include "Test.hpp"

#include <memory>

#include "Utils.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"

namespace test {
class TestMovement2D : public Test {
private:
    std::unique_ptr<VertexArray> m_va;
    std::unique_ptr<VertexBuffer> m_vb;
    std::unique_ptr<IndexBuffer> m_ib;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Texture> m_texture;

    glm::mat4 m_proj, m_view, m_model, m_mvp;
    glm::vec3 m_translation;
    
    ImGuiIO& io;
    GLFWwindow* window;
    int speed;
public:
    TestMovement2D()
        : m_proj(glm::ortho(-480.0f, 480.0f, -270.0f, 270.0f, -500.0f, 500.0f)),
        m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))),
        io(ImGui::GetIO()), window(glfwGetCurrentContext()),
        speed(5)
    {
        float vertices[] = {
            -50.0f, -50.0f, 0.0f, 0.0f,
             50.0f, -50.0f, 1.0f, 0.0f,
             50.0f,  50.0f, 1.0f, 1.0f,
            -50.0f,  50.0f, 0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_va = std::make_unique<VertexArray>();
        m_vb = std::make_unique<VertexBuffer>(vertices, 4*4 * sizeof(float));
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        m_va->add_buffer(*m_vb, layout);

        m_ib = std::make_unique<IndexBuffer>(indices, 6);

        m_shader = std::make_unique<Shader>("../src/workspace/shaders/texture.glsl");
        m_shader->bind();

        m_texture = std::make_unique<Texture>("../res/textures/shrek.png");
        m_texture->bind();
        m_shader->set_uniform_1i("u_texture", 0);
    }
    ~TestMovement2D() {}

    

    virtual void on_update(float deltaTime) override {
        io.AddKeyEvent(ImGuiKey_W, (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS));
        io.AddKeyEvent(ImGuiKey_A, (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS));
        io.AddKeyEvent(ImGuiKey_S, (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS));
        io.AddKeyEvent(ImGuiKey_D, (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS));

        if (io.KeysDown[ImGuiKey_W]) m_translation.y += speed;
        if (io.KeysDown[ImGuiKey_A]) m_translation.x -= speed;
        if (io.KeysDown[ImGuiKey_S]) m_translation.y -= speed;
        if (io.KeysDown[ImGuiKey_D]) m_translation.x += speed;
    }
    virtual void on_render() override {
        Renderer renderer;
        renderer.clear();

        {
            m_model = glm::translate(glm::mat4(1.0f), m_translation);
            m_mvp = m_proj * m_view * m_model;
            m_shader->set_uniform_mat_4f("u_MVP", m_mvp);
            renderer.draw(*m_va, *m_ib, *m_shader);
        }
    }
    virtual void on_imgui_render() override {
        {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 500.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
    }
};
}

#endif