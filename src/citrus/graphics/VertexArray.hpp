#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include "Keywords.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray {
private:
    unsigned int m_RendererID;
public:
    VertexArray() {
        GLCall(glGenVertexArrays(1, &m_RendererID));
    }
    ~VertexArray() {
        GLCall(glDeleteVertexArrays(1, &m_RendererID));
    }

    void add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
        bind();
        const auto& elements = layout.get_elements();
        unsigned int offset = 0;
        for (unsigned int i = 0; i < elements.size(); i++) {
            const auto& element = elements[i];
            GLCall(glEnableVertexAttribArray(i));
            GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, element.count*layout.get_stride(), reinterpret_cast<const void*>(offset)));
            offset += element.count * VertexBufferElement::get_sizeof(element.type);
        }
    }

    void bind() const {
        GLCall(glBindVertexArray(m_RendererID));
    }

    void unbind() const {
        GLCall(glBindVertexArray(0));
    }
};
#endif