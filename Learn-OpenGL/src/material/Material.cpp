#include "Material.h"
#include "../Texture.h"

Material::Material(const Shader& shader, const glm::vec3& translateVec, const glm::vec3& scaleVec, 
	const glm::vec3& rotationDirections, const float rotationSpeed) 
	: m_Shader(shader), m_RotationDirections(rotationDirections), m_RotationSpeed(rotationSpeed)
{
	shader.bind();
	m_ModelMat = glm::translate(glm::mat4(1.0f), translateVec);
	m_ModelMat = glm::scale(m_ModelMat, scaleVec);
	shader.setUniformMat4f("u_ModelMat", m_ModelMat);
}

void Material::updateModelMat(float frameTime)
{
	m_Shader.bind();
	// Rotation around the object's center
	m_ModelMat = glm::rotate(m_ModelMat, frameTime * m_RotationSpeed, m_RotationDirections);
	// Rotation around the world's center
	m_ModelMat = glm::rotate(glm::mat4(1.0f), frameTime * m_RotationSpeed, m_RotationDirections) * m_ModelMat;
	m_Shader.setUniformMat4f("u_ModelMat", m_ModelMat);
}

void Material::updateViewMat(const glm::mat4& viewMat) const
{
	m_Shader.bind();
	m_Shader.setUniformMat4f("u_ViewMat", viewMat);
}

void Material::updateProjectionMat(const glm::mat4& projectionMat) const
{
	m_Shader.bind();
	m_Shader.setUniformMat4f("u_ProjectionMat", projectionMat);
}
