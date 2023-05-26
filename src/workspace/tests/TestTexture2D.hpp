#ifndef TEST_TEXTURE_2D_HPP
#define TEST_TEXTURE_2D_HPP

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
class TestTexture2D : public Test {
private:
    std::unique_ptr<VertexArray> m_va;
    std::unique_ptr<VertexBuffer> m_vb;
    std::unique_ptr<IndexBuffer> m_ib;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Texture> m_texture;

    glm::mat4 m_proj, m_view, m_model, m_mvp;
    glm::vec3 m_translationA, m_translationB;
public:
    TestTexture2D() 
        : m_proj(glm::ortho(-480.0f, 480.0f, -270.0f, 270.0f, -500.0f, 500.0f)),
        m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
        m_translationA(-100.0f, -100.0f, 0.0f), m_translationB(100.0f, 100.0f, 0.0f)
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
    ~TestTexture2D() {}

    virtual void on_update(float deltaTime) override {}
    virtual void on_render() override {
        Renderer renderer;
        renderer.clear();

        {
            m_model = glm::translate(glm::mat4(1.0f), m_translationA);
            m_mvp = m_proj * m_view * m_model;
            m_shader->set_uniform_mat_4f("u_MVP", m_mvp);
            renderer.draw(*m_va, *m_ib, *m_shader);
        }
            
        {
            m_model = glm::translate(glm::mat4(1.0f), m_translationB);
            m_mvp = m_proj * m_view * m_model;
            m_shader->set_uniform_mat_4f("u_MVP", m_mvp);
            renderer.draw(*m_va, *m_ib, *m_shader);
        }
    }
    virtual void on_imgui_render() override {
        {
            ImGui::SliderFloat3("Translation A", &m_translationA.x, -270.0f, 270.0f);
            ImGui::SliderFloat3("Translation B", &m_translationB.x, -270.0f, 270.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 500.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
    }
};
}

#endif