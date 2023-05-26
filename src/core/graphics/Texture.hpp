#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Utils.hpp"
#include "stb_image/stb_image.h"

class Texture {
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BitsPerPixel;
public:
    Texture(const std::string& path)
    : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
    m_Width(0), m_Height(0), m_BitsPerPixel(0) {
        stbi_set_flip_vertically_on_load(1);
        m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BitsPerPixel, 4);

        GLCall(glGenTextures(1, &m_RendererID));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));

        if (m_LocalBuffer)
            stbi_image_free(m_LocalBuffer);
    }
    ~Texture() {
        GLCall(glDeleteTextures(1, &m_RendererID));
    }

    void bind(unsigned int slot = 0) const {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
    }
    void unbind() const {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }

    inline int get_width() const { return m_Width;}
    inline int get_height() const { return m_Height;}
};

#endif