#include "Material.h"
#include "Texture.h"

#include <GLFW/glfw3.h>

Material::Material(const Shader& shader, const glm::mat4& scaleMat, const glm::mat4& rotateMat, const glm::mat4& translateMat, 
	const float modelRotationSpeed, const glm::vec3& modelRotationDirections, const glm::mat4& viewMat, 
	const glm::mat4& projectionMat) : m_Shader(shader)
{
	shader.bind();
	glm::mat4 modelMat = scaleMat * rotateMat * translateMat;
	modelMat = glm::rotate(modelMat, (float)glfwGetTime() * modelRotationSpeed, modelRotationDirections);

	glm::mat4 mvp = projectionMat * viewMat * modelMat;

	shader.setUniformMat4f("u_MVP", mvp);
}

void Material::setMaterialColor(const glm::vec4& color) const
{
	m_Shader.setUniform1i("u_UseTexture", false);
	m_Shader.setUniform4f("u_Color", color.r, color.g, color.b, color.a);
}

void Material::setMaterialTexture(const int texSlot, const float texOpacity) const
{
	m_Shader.setUniform1i("u_Texture", texSlot);
	m_Shader.setUniform1i("u_UseTexture", true);
	m_Shader.setUniform1f("u_TexOpacity", texOpacity);
}
