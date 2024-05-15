#pragma once
#include "../Shader.h"
#include "Material.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class ColorMaterial : public Material
{
private:
	glm::vec4 m_Color;
public:
	ColorMaterial(const Shader& shader, const glm::vec3& translateVec, const glm::vec3& scaleVec,
		const glm::vec3& rotationDirections, const float rotationSpeed, const glm::vec4& color);
	void setColorTexture() const override;
};