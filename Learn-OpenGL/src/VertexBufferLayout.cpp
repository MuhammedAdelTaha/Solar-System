#include "VertexBufferLayout.h"

void VertexBufferLayout::addAttrib(VertexAttribute attribute)
{
	m_Attributes.push_back(attribute);
	m_Stride += attribute.getAttribSize();
}

void VertexBufferLayout::addAttributes(vector<VertexAttribute> attributes)
{
	for (VertexAttribute& attribute : attributes)
	{
		addAttrib(attribute);
	}
}
