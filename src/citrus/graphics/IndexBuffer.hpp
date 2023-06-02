#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include "Utils.hpp"

class IndexBuffer {
private:
    unsigned int
        m_RendererID,
        m_Count;
public:
    IndexBuffer(const unsigned int* data, unsigned int count) :
    m_Count(count) {
        ASSERT(sizeof(unsigned int) == sizeof(GLuint));

        GLCall(glGenBuffers(1, &m_RendererID));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW));
    }
    ~IndexBuffer() {
        GLCall(glDeleteBuffers(1, &m_RendererID));
    }

    void bind() const {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    }
    void unbind() const {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    inline unsigned int get_count() const {return m_Count;}
};

#endif