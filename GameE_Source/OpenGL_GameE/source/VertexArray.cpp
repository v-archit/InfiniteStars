#include"VertexArray.h"
#include"Renderer.h"
#include"VertexBufferLayout.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);        //generate vertex array
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{

	Bind();            //bind vertex array
	vb.Bind();             //bind vertex buffer
	const auto& elements = layout.GetElements();         //2
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);           //enable vertex attribute
		glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void*)offset); //pointer to vertex attribute (convey the struct of vertices) - layout of vertex buffer
		offset += element.count * VBLayoutElement::GetSizeOfType(element.type);
	}

		
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);            //bind vertex array

}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
