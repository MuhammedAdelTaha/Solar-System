#include "ElementBuffer.h"
#include "Debugger.h"

ElementBuffer::ElementBuffer(unsigned int count, const void* data) : m_count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLint));

	// (number of buffer object names to be generated, pointer to array in which the generated object names are stored)
	glCall(glGenBuffers(1, &m_RendererId));

	// (the target in which the buffer object is bound, the name of a buffer object)
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));

	// (target, data size in bytes, data, usage)
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &m_RendererId);
}

void ElementBuffer::bind() const
{
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
}

void ElementBuffer::unbind() const
{
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
