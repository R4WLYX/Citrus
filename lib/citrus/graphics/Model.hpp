#ifndef MODEL_HPP
#define MODEL_HPP

#include <memory>

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"

class Model {
public:
    std::unique_ptr<VertexArray> vertexArray;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::unique_ptr<Texture> texture;

    Model(float* vertices, unsigned int* indices, size_t verticesCount, size_t indicesCount, VertexBufferLayout layout, Texture modelTexture)
     : texture(std::make_unique<Texture>(modelTexture))
    {
        vertexArray = std::make_unique<VertexArray>();
        vertexBuffer = std::make_unique<VertexBuffer>(vertices, verticesCount * sizeof(float));
        vertexArray->add_buffer(*vertexBuffer, layout);
        indexBuffer = std::make_unique<IndexBuffer>(indices, indicesCount);
    }
};

#endif