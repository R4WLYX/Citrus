#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "Keywords.hpp"

class VertexBuffer {
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data, unsigned int size) {
        GLCall(glGenBuffers(1, &m_RendererID));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }
    ~VertexBuffer() {
        GLCall(glDeleteBuffers(1, &m_RendererID));
    }

    void bind() const {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    }
    void unbind() const {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
};

#endif