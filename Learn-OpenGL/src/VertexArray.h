#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
	unsigned int m_RendererId;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout) const;
	void bind() const;
	void unbind() const;
};