#pragma once
#include <vector>
using namespace std;

struct VertexAttribute {
	// m_Count (number of components), m_Type (attribute type), m_Normalized (Is the attribute normalized or not)
	unsigned int m_Count, m_Type, m_Normalized;

	VertexAttribute(unsigned int count, unsigned int type, unsigned int normalized) {
		m_Count = count;
		m_Type = type;
		m_Normalized = normalized;
	}
	
	unsigned int getAttribSize() const
	{
		unsigned int attribSize = m_Count;
		switch (m_Type)
		{
			case 5120: // byte
				attribSize *= sizeof(char);
				break;
			case 5121: // unsigned byte
				attribSize *= sizeof(unsigned char);
				break;
			case 5122: // short
				attribSize *= sizeof(short);
				break;
			case 5123: // unsigned short
				attribSize *= sizeof(unsigned short);
				break;
			case 5124: // int
				attribSize *= sizeof(int);
				break;
			case 5125: // unsigned int
				attribSize *= sizeof(unsigned int);
				break;
			case 5126: // float 
				attribSize *= sizeof(float);
				break;
			case 5130: // double
				attribSize *= sizeof(double);
				break;
			default:
				break;
		}
		return attribSize;
	}
};

class VertexBufferLayout
{
	vector<VertexAttribute> m_Attributes;
	unsigned int m_Stride;

public:
	VertexBufferLayout() : m_Stride(0) {};
	void addAttrib(VertexAttribute attribute);
	void addAttributes(vector<VertexAttribute> attributes);

	inline vector<VertexAttribute> getAttributes() const { return m_Attributes; };
	inline unsigned int getStride() const { return m_Stride; };
};
