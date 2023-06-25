#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;    //member  //id for every object created in opengl - texture, shader, buffer etc. 
	unsigned int m_Count;        //indices
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const
	{
		return m_Count;
	}
};