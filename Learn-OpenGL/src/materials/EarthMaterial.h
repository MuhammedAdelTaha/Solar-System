#pragma once
#include "../Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class EarthMaterial
{
	Shader m_Shader;
	glm::mat4 m_ViewMat;
	glm::mat4 m_ProjectionMat;

public:
	EarthMaterial(const Shader& shader, const glm::mat4& viewMat, const glm::mat4& projectionMat, const int texSlot);
	Shader getShader() const;
	glm::mat4 getViewMat() const;
	glm::mat4 getProjectionMat() const;
};