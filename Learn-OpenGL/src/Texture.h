#pragma once
#include <string>
using namespace std;

class Texture
{
	string m_FilePath;
	unsigned int m_RendererId;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const string& filePath);

	void bind(unsigned int slot = 0) const;
	void unbind() const;
	void deleteTexture() const;

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
	inline int getBPP() const { return m_BPP; }

};