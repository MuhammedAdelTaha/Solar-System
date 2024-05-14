#pragma once
#include "../Shader.h"

class RockMaterial
{
	Shader m_Shader;
	glm::mat4 m_ViewMat;
	glm::mat4 m_ProjectionMat;
public:
	RockMaterial(const Shader& shader, const glm::mat4& m_ViewMat, const glm::mat4& m_ProjectionMat, const int texSlot);
	Shader getShader() const;
	glm::mat4 getViewMat() const;
	glm::mat4 getProjectionMat() const;
};