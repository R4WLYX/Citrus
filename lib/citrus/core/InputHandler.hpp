#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <map>

#define KEY_UP           0
#define KEY_DOWN         1
#define KEY_PRESSED      2
#define KEY_RELEASED     3

class InputHandler {
private:
    static std::map<int, const char*> KeyCode;
    static std::unordered_map<const char*, int> KeyMap;
    
public:
    InputHandler(GLFWwindow* window);

    std::unordered_map<const char*, int> get_key_map() { return KeyMap; }

    void update_key_map() {
        for (auto& pair : KeyMap) {
            if (pair.second == KEY_DOWN) {
                pair.second = KEY_PRESSED;
            } else if (pair.second == KEY_UP) {
                pair.second = KEY_RELEASED;
            }
        }
    }
};

std::map<int, const char*> InputHandler::KeyCode = {
    {32, "SPACE"},
    {39, "APOSTROPHE"},
    {44, "COMMA"},
    {45, "MINUS"},
    {46, "PERIOD"},
    {47, "SLASH"},
    {48, "0"},
    {49, "1"},
    {50, "2"},
    {51, "3"},
    {52, "4"},
    {53, "5"},
    {54, "6"},
    {55, "7"},
    {56, "8"},
    {57, "9"},
    {59, "SEMICOLON"},
    {61, "EQUAL"},
    {65, "A"},
    {66, "B"},
    {67, "C"},
    {68, "D"},
    {69, "E"},
    {70, "F"},
    {71, "G"},
    {72, "H"},
    {73, "I"},
    {74, "J"},
    {75, "K"},
    {76, "L"},
    {77, "M"},
    {78, "N"},
    {79, "O"},
    {80, "P"},
    {81, "Q"},
    {82, "R"},
    {83, "S"},
    {84, "T"},
    {85, "U"},
    {86, "V"},
    {87, "W"},
    {88, "X"},
    {89, "Y"},
    {90, "Z"},
    {91, "LEFT_BRACKET"},
    {92, "BACKSLASH"},
    {93, "RIGHT_BRACKET"},
    {96, "GRAVE_ACCENT"},
    {161, "WORLD_1"},
    {162, "WORLD_2"},
    {256, "ESCAPE"},
    {257, "ENTER"},
    {258, "TAB"},
    {259, "BACKSPACE"},
    {260, "INSERT"},
    {261, "DELETE"},
    {262, "RIGHT"},
    {263, "LEFT"},
    {264, "DOWN"},
    {265, "UP"},
    {266, "PAGE_UP"},
    {267, "PAGE_DOWN"},
    {268, "HOME"},
    {269, "END"},
    {280, "CAPS_LOCK"},
    {281, "SCROLL_LOCK"},
    {282, "NUM_LOCK"},
    {283, "PRINT_SCREEN"},
    {284, "PAUSE"},
    {290, "F1"},
    {291, "F2"},
    {292, "F3"},
    {293, "F4"},
    {294, "F5"},
    {295, "F6"},
    {296, "F7"},
    {297, "F8"},
    {298, "F9"},
    {299, "F10"},
    {300, "F11"},
    {301, "F12"},
    {302, "F13"},
    {303, "F14"},
    {304, "F15"},
    {305, "F16"},
    {306, "F17"},
    {307, "F18"},
    {308, "F19"},
    {309, "F20"},
    {310, "F21"},
    {311, "F22"},
    {312, "F23"},
    {313, "F24"},
    {314, "F25"},
    {320, "KP_0"},
    {321, "KP_1"},
    {322, "KP_2"},
    {323, "KP_3"},
    {324, "KP_4"},
    {325, "KP_5"},
    {326, "KP_6"},
    {327, "KP_7"},
    {328, "KP_8"},
    {329, "KP_9"},
    {330, "KP_DECIMAL"},
    {331, "KP_DIVIDE"},
    {332, "KP_MULTIPLY"},
    {333, "KP_SUBTRACT"},
    {334, "KP_ADD"},
    {335, "KP_ENTER"},
    {336, "KP_EQUAL"},
    {340, "LEFT_SHIFT"},
    {341, "LEFT_CONTROL"},
    {342, "LEFT_ALT"},
    {343, "LEFT_SUPER"},
    {344, "RIGHT_SHIFT"},
    {345, "RIGHT_CONTROL"},
    {346, "RIGHT_ALT"},
    {347, "RIGHT_SUPER"},
    {348, "MENU"}
};

std::unordered_map<const char*, int> InputHandler::KeyMap;

InputHandler::InputHandler(GLFWwindow* window) {
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        const char* keyCode = KeyCode[key];
        if (action == GLFW_PRESS)
            KeyMap[keyCode] = KEY_DOWN;
        else if (action == GLFW_RELEASE)
            KeyMap[keyCode] = KEY_UP;
    });
}

#endif