#pragma once
#include "../Shader.h"

class SpaceMaterial
{
	Shader m_Shader;
	glm::mat4 m_ViewMat;
	glm::mat4 m_ProjectionMat;
public:
	SpaceMaterial(const Shader& shader, const glm::mat4& m_ViewMat, const glm::mat4& m_ProjectionMat, const float offset,
		const float opacity, const int texSlot, const int useTex);
	Shader getShader() const;
	glm::mat4 getViewMat() const;
	glm::mat4 getProjectionMat() const;
};