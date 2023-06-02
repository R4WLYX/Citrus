#ifndef TEST_HPP
#define TEST_HPP

#include <vector>
#include <functional>

#include "imgui/imgui.h"

namespace test {
class Test {
public:
    Test() {}
    virtual ~Test() {}

    virtual void on_update(float deltaTime) {}
    virtual void on_render() {}
    virtual void on_imgui_render() {}
};

class TestMenu : public Test {
private:
    Test*& m_currentTest;
    std::vector<std::pair<std::string, std::function<Test*()>>> m_tests;
public:
    TestMenu(Test*& currentTestPointer)
        : m_currentTest(currentTestPointer)
    {}

    void on_imgui_render() override {
        for (auto& test : m_tests) {
            if (ImGui::Button(test.first.c_str()))
                m_currentTest = test.second();
        }
    }

    template<typename T>
    void register_test(const std::string& name) {
        m_tests.push_back(std::make_pair(name, [](){ return new T(); }));
    }
};
}

#endif