#include "Debugger.h"
#include "Renderer.h"

void Renderer::draw(const VertexArray& vao, const ElementBuffer& ebo, const Shader& shader) const
{
	vao.bind();
	ebo.bind();
	shader.bind();
	glCall(glDrawElements(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

