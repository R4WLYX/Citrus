#ifndef VERTEX_BUFFER_LAYOUT_HPP
#define VERTEX_BUFFER_LAYOUT_HPP

#include <vector>
#include "Utils.hpp"

struct VertexBufferElement {
    VertexBufferElement(unsigned int type, unsigned int count, unsigned char normalized) :
    type(type), count(count), normalized(normalized) {}

    unsigned int type, count;
    unsigned char normalized;

    static unsigned int get_sizeof(unsigned int type) {
        switch (type) {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout() :
    m_Stride(0) {}
    ~VertexBufferLayout() {}

    template<typename T>
    void push(unsigned int count) {}

    inline const std::vector<VertexBufferElement>& get_elements() const {return m_Elements;}
    inline unsigned int get_stride() const {return m_Stride;}
};

template<>
void VertexBufferLayout::push<float>(unsigned int count) {
    m_Elements.push_back(VertexBufferElement(GL_FLOAT, count, GL_FALSE));
    m_Stride += VertexBufferElement::get_sizeof(GL_FLOAT);
}

template<>
void VertexBufferLayout::push<unsigned int>(unsigned int count) {
    m_Elements.push_back(VertexBufferElement(GL_UNSIGNED_INT, count, GL_FALSE));
    m_Stride += VertexBufferElement::get_sizeof(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::push<unsigned char>(unsigned int count) {
    m_Elements.push_back(VertexBufferElement(GL_UNSIGNED_BYTE, count, GL_TRUE));
    m_Stride += VertexBufferElement::get_sizeof(GL_UNSIGNED_BYTE);
}

#endif