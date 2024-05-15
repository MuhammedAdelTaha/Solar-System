#pragma once
#include "../Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Material
{
protected:
	Shader m_Shader;
private:
	glm::mat4 m_ModelMat;
	glm::vec3 m_RotationDirections;
	float m_RotationSpeed;
public:
	Material(const Shader& shader, const glm::vec3& translateVec, const glm::vec3& scaleVec,
		const glm::vec3& rotationDirections, const float rotationSpeed);
	void updateModelMat(float frameTime);
	void updateViewMat(const glm::mat4& viewMat) const;
	void updateProjectionMat(const glm::mat4& projectionMat) const;

	virtual void setColorTexture() const = 0;

	inline const Shader getShaderId() const { return m_Shader; }
};