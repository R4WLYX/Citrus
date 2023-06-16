#ifndef KEYWORDS_HPP
#define KEYWORDS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) gl_clear_error(); x; ASSERT(gl_log_call(#x, __FILE__, __LINE__))

static void gl_clear_error() {
    while(glGetError() != GL_NO_ERROR);
}

static bool gl_log_call(const char* function, const char* file, int line) {
    while(GLenum error = glGetError()) {
        std::cout<<"[OpenGL Error] ("<<error<<"): "<<function<<' '<<file<<':'<<line<<'\n';
        return false;
    }
    return true;
}

#endif