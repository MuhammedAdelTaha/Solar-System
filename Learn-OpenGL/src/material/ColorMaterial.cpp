#include "ColorMaterial.h"
#include "../Texture.h"

ColorMaterial::ColorMaterial(const Shader& shader, const glm::vec3& translateVec, const glm::vec3& scaleVec,
	const glm::vec3& rotationDirections, const float rotationSpeed, const glm::vec4& color)
	: Material(shader, translateVec, scaleVec, rotationDirections, rotationSpeed), m_Color(color) {};

void ColorMaterial::setColorTexture() const
{
	m_Shader.bind();
	m_Shader.setUniform4f("u_Color", m_Color.r, m_Color.g, m_Color.b, m_Color.a);
}