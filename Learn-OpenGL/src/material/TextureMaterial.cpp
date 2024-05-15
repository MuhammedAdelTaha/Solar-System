#include "TextureMaterial.h"
#include <iostream>

TextureMaterial::TextureMaterial(const Shader& shader, const glm::vec3& translateVec, const glm::vec3& scaleVec,
	const glm::vec3& rotationDirections, const float rotationSpeed, const Texture& texture, const int texSlot, 
	const float texOpacity) :
	Material(shader, translateVec, scaleVec, rotationDirections, rotationSpeed),
	m_Texture(texture), m_TexSlot(texSlot), m_TexOpacity(texOpacity) {};

void TextureMaterial::setColorTexture() const
{
	m_Shader.bind();
	m_Texture.bind(m_TexSlot);
	m_Shader.setUniform1i("u_Texture", m_TexSlot);
	m_Shader.setUniform1f("u_TexOpacity", m_TexOpacity);
}
