#pragma once
#include "../Shader.h"
#include "../Texture.h"
#include "Material.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class TextureMaterial : public Material
{
private:
	Texture m_Texture;
	int m_TexSlot;
	float m_TexOpacity;
public:
	TextureMaterial(const Shader& shader, const glm::vec3& translateVec, const glm::vec3& scaleVec,
		const glm::vec3& rotationDirections, const float rotationSpeed, const Texture& texture, const int texSlot,
		const float texOpacity);
	void setColorTexture() const override;
};