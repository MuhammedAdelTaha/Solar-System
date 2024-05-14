#include "SpaceMaterial.h"
#include "../Texture.h"

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

SpaceMaterial::SpaceMaterial(const Shader& shader, const glm::mat4& viewMat, const glm::mat4& projectionMat, const float offset,
	const float opacity, const int texSlot, const int useTex) : m_Shader(shader), m_ViewMat(viewMat), m_ProjectionMat(projectionMat)
{
	shader.bind();

	glm::mat4 modelMat = glm::scale(glm::mat4(1.0f), glm::vec3(400.0f + offset, 400.0f + offset, 400.0f + offset));
	glm::mat4 mvp = projectionMat * viewMat * modelMat;

	shader.setUniformMat4f("u_MVP", mvp);
	shader.setUniform1f("u_Opacity", opacity);
	shader.setUniform1i("u_Texture", texSlot);
	shader.setUniform1i("u_UseTexture", useTex);
}

Shader SpaceMaterial::getShader() const
{
	return m_Shader;
}

glm::mat4 SpaceMaterial::getViewMat() const
{
	return m_ViewMat;
}

glm::mat4 SpaceMaterial::getProjectionMat() const
{
	return m_ProjectionMat;
}
