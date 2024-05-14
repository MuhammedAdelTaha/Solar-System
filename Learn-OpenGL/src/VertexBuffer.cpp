#include "VertexBuffer.h"
#include "Debugger.h"

VertexBuffer::VertexBuffer(unsigned int size, const void* data)
{
	// (number of buffer object names to be generated, pointer to array in which the generated object names are stored)
	glCall(glGenBuffers(1, &m_RendererId));

	// (the target in which the buffer object is bound, the name of a buffer object)
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
	
	// (target, data size in bytes, data, usage)
	glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererId);
}

void VertexBuffer::bind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}

void VertexBuffer::unbind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
