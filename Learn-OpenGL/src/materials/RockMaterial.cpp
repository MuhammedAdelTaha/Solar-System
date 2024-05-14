#include "RockMaterial.h"
#include "../Texture.h"

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

RockMaterial::RockMaterial(const Shader& shader, const glm::mat4& viewMat, const glm::mat4& projectionMat, const int texSlot)
	: m_Shader(shader), m_ViewMat(viewMat), m_ProjectionMat(projectionMat)
{
	shader.bind();

	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -2.0f, -5.0f));

	glm::mat4 modelMat = scaleMat * translateMat;
	modelMat = glm::rotate(modelMat, (float)glfwGetTime() * 1.0f, glm::vec3(5.0f, 1.0f, 0.0f));

	glm::mat4 mvp = projectionMat * viewMat * modelMat;

	shader.setUniformMat4f("u_MVP", mvp);
	shader.setUniform1f("u_Opacity", 1.0f);
	shader.setUniform1i("u_UseTexture", 1);
	shader.setUniform1i("u_Texture", texSlot);
}

Shader RockMaterial::getShader() const
{
	return m_Shader;
}

glm::mat4 RockMaterial::getViewMat() const
{
	return m_ViewMat;
}

glm::mat4 RockMaterial::getProjectionMat() const
{
	return m_ProjectionMat;
}
