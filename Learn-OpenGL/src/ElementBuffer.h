#pragma once

class ElementBuffer
{
	unsigned int m_RendererId; // The EBO id
	unsigned int m_count; // The number of elements/indices
public:
	ElementBuffer(unsigned int count, const void* data);
	~ElementBuffer();

	void bind() const;
	void unbind() const;
	inline unsigned int getCount() const { return m_count; };
};
