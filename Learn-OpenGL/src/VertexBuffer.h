#pragma once

class VertexBuffer
{
	unsigned int m_RendererId; // The VBO id
public:
	VertexBuffer(unsigned int size, const void* data);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};
