#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Keywords.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer {
public:
    void clear() const {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
        shader.bind();
        va.bind();
        ib.bind();
        GLCall(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));
    }
};

#endif