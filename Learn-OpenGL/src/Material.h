#pragma once
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Material
{
private:
	Shader m_Shader;
public:
	Material(const Shader& shader, const glm::mat4& scaleMat, const glm::mat4& rotateMat, const glm::mat4& translateMat,
		const float modelRotationSpeed, const glm::vec3& modelRotationDirections, const glm::mat4& viewMat,
		const glm::mat4& projectionMat);
	~Material() = default;
	void setMaterialColor(const glm::vec4& color) const;
	void setMaterialTexture(const int texSlot, const float texOpacity) const;
};