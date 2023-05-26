#ifndef TEST_CLEAR_COLOR_HPP
#define TEST_CLEAR_COLOR_HPP

#include "Test.hpp"

#include "Utils.hpp"
#include "imgui/imgui.h"

namespace test {
class TestClearColor : public Test {
private:
    float m_clearColor[4];
public:
    TestClearColor() 
        : m_clearColor{1.0f, 1.0f, 1.0f, 1.0f}
    {}
    ~TestClearColor() {}

    virtual void on_update(float deltaTime) override {}
    virtual void on_render() override {
        GLCall(glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }
    virtual void on_imgui_render() override {
        ImGui::ColorEdit4("Clear Color", m_clearColor);
    }
};
}

#endif