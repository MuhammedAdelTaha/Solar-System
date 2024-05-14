#include "EarthMaterial.h"
#include "../Texture.h"

#include <GLFW/glfw3.h>

EarthMaterial::EarthMaterial(const Shader& shader, const glm::mat4& viewMat, const glm::mat4& projectionMat, const int texSlot)
	: m_Shader(shader), m_ViewMat(viewMat), m_ProjectionMat(projectionMat)
{
	shader.bind();
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	//glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, -10.0f));

	glm::mat4 modelMat = scaleMat * translateMat;
	modelMat = glm::rotate(modelMat, (float)glfwGetTime() * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 mvp = m_ProjectionMat * m_ViewMat * modelMat;

	shader.setUniformMat4f("u_MVP", mvp);
	shader.setUniform1f("u_Opacity", 1.0f);
	shader.setUniform1i("u_UseTexture", 1);
	shader.setUniform1i("u_Texture", texSlot);
};

Shader EarthMaterial::getShader() const
{
	return m_Shader;
}

glm::mat4 EarthMaterial::getViewMat() const
{
	return m_ViewMat;
}

glm::mat4 EarthMaterial::getProjectionMat() const
{
	return m_ProjectionMat;
}