#ifndef TEST_BATCH_RENDERING_HPP
#define TEST_BATCH_RENDERING_HPP

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
class TestBatchRendering : public Test {
private:
    std::unique_ptr<VertexArray> m_va;
    std::unique_ptr<VertexBuffer> m_vb;
    std::unique_ptr<IndexBuffer> m_ib;
    std::unique_ptr<Shader> m_shader;
public:
    TestBatchRendering() {
        float vertices[] = {
            -50.0f, -50.0f,
             50.0f, -50.0f,
             50.0f,  50.0f,
            -50.0f,  50.0f,

             50.0f,  50.0f,
            -50.0f,  50.0f,
            -50.0f, -50.0f,
             50.0f, -50.0f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_va = std::make_unique<VertexArray>();
        m_vb = std::make_unique<VertexBuffer>(vertices, 8*2 * sizeof(float));
        VertexBufferLayout layout;
        layout.push<float>(4);
        m_va->add_buffer(*m_vb, layout);

        m_ib = std::make_unique<IndexBuffer>(indices, 6);

        m_shader = std::make_unique<Shader>("../src/workspace/shaders/basic.glsl");
        m_shader->bind();
    }
    ~TestBatchRendering() {}

    virtual void on_update(float deltaTime) override {}
    virtual void on_render() override {
        Renderer renderer;
        renderer.clear();

        renderer.draw(*m_va, *m_ib, *m_shader);
    }
    virtual void on_imgui_render() override {
        {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 500.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
    }
};
}

#endif