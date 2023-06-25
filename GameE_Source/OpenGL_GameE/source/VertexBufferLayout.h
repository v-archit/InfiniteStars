#pragma once

#include<vector>
#include"Renderer.h"

struct VBLayoutElement
{
	unsigned int type;          //element type
	unsigned int count;         //element count
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VBLayoutElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0)
	{

	}

	template<typename T>
	void Push(unsigned int count)
	{
		//static_assert(false);             //if types is unmatched
	}
	
	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VBLayoutElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VBLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);

	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VBLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);

	}

	const std::vector<VBLayoutElement> GetElements() const
	{
		return m_Elements;
	}

	unsigned int GetStride() const
	{
		return m_Stride;
	}
};