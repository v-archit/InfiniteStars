#include"IndexBuffer.h"
#include"Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count)        //m_Count(count) -> member initializer list
{
    glGenBuffers(1, &m_RendererID);             //buffer generation
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);          //buffer binding to GPU
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);     //buffer fill data - 4 for 4 vertex positions 
}
   
IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
} 

void IndexBuffer::Bind() const
{
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);          //buffer binding to GPU

}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);          //buffer binding to GPU

}
