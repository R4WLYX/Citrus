#include "Renderer.hpp"
#include "Window.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.hpp"
#include "tests/TestTexture2D.hpp"
#include "tests/TestMovement2D.hpp"
#include "tests/TestStress.hpp"

int main() {
    Window window("Hello world!");
    GLFWwindow* glfwWindow = window.get_glfw_window();

    {
        Renderer renderer;

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->register_test<test::TestClearColor>("Clear Color");
        testMenu->register_test<test::TestTexture2D>("2D Texture");
        testMenu->register_test<test::TestMovement2D>("2D Movement");
        testMenu->register_test<test::TestStress>("Stress test");

        while (!glfwWindowShouldClose(glfwWindow)) {
            glfwPollEvents();
            renderer.clear();
            
            currentTest->on_update(500.0f / ImGui::GetIO().Framerate);
            currentTest->on_render();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Tests Menu");
            if (currentTest != testMenu && ImGui::Button("<-")) {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->on_imgui_render();
            ImGui::End();
            
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(glfwWindow);
        }
        
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}