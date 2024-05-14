#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
using namespace std;

struct BasicShaders { string vertexShaderSrc, fragmentShaderSrc; };

class Shader
{
	string m_FilePath;
	unsigned int m_RendererId;
	unordered_map<string, int> m_UniformLocationCache;
public:
	Shader(const string& filepath);
	~Shader();
	void bind() const;
	void unbind() const;
	void setUniform1i(const string& name, int v0) const;
	void setUniform1f(const string& name, float v0) const;
	void setUniform2f(const string& name, float v0, float v1) const;
	void setUniform3f(const string& name, float v0, float v1, float v2) const;
	void setUniform4f(const string& name, float v0, float v1, float v2, float v3) const;
	void setUniformMat4f(const string& name, const glm::mat4& matrix) const;
private:
	BasicShaders parseBasicShaders(const string& filepath) const;
	unsigned int createShader(unsigned int type, const string& source) const;
	unsigned int createProgram(const string& vertexShaderSrc, const string& fragmentShaderSrc) const;
	int getUniformLocation(const string& name) const;
};
