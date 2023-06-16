#include <iostream>

#include "Renderer.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.hpp"
#include "tests/TestTexture2D.hpp"
#include "tests/TestMovement2D.hpp"

int main() { 
    GLFWwindow* window;
    GLFWmonitor* monitor;
    int monitorWidth, monitorHeight;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        glfwTerminate();
        return -1;
    }

    glfwGetMonitorWorkarea(monitor, NULL, NULL, &monitorWidth, &monitorHeight);

    window = glfwCreateWindow(monitorWidth, monitorHeight, "Hello World! :D", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error\n";

    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->register_test<test::TestClearColor>("Clear Color");
        testMenu->register_test<test::TestTexture2D>("2D Texture");
        testMenu->register_test<test::TestMovement2D>("2D Movement");

        while (!glfwWindowShouldClose(window)) {
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

            glfwSwapBuffers(window);
        }
        
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}