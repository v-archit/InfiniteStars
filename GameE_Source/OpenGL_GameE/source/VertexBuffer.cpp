#include"VertexBuffer.h"
#include"Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &m_RendererID);             //buffer generation
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);          //buffer binding to GPU
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);     //buffer fill data 
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
} 

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);          //buffer binding to GPU

}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);          //buffer binding to GPU

}
