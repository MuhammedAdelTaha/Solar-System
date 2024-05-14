#include "VertexArray.h"
#include "Debugger.h"

VertexArray::VertexArray()
{
	glCall(glGenVertexArrays(1, &m_RendererId));
}

VertexArray::~VertexArray()
{
	glCall(glDeleteVertexArrays(1, &m_RendererId));
}

void VertexArray::addBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout) const
{
	bind();
	vbo.bind();
	const vector<VertexAttribute>& attributes = layout.getAttributes();
	for (unsigned int i = 0, offset = 0; i < attributes.size(); i++) {
		VertexAttribute attribute = attributes[i];
		
		// (the index of the attribute to be enabled (0, 1, 2, ...) (vertex position has index 0))
		glCall(glEnableVertexAttribArray(i));

		// (attribute index, number of components in attribute, type of attribute, normalized or not, stride (size of vertex), 
		// pointer (offset of attribute)(0 for the first attribute))
		glCall(glVertexAttribPointer(i, attribute.m_Count, attribute.m_Type, attribute.m_Normalized, layout.getStride(), (const void*)offset));
		offset += attribute.getAttribSize();
	}
}

void VertexArray::bind() const
{
	glCall(glBindVertexArray(m_RendererId));
}

void VertexArray::unbind() const
{
	glCall(glBindVertexArray(0));
}
