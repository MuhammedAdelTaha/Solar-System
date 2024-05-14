#pragma once
#include "VertexArray.h"
#include "ElementBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	void draw(const VertexArray& vao, const ElementBuffer& ebo, const Shader& shader) const;
	void clear() const;
};