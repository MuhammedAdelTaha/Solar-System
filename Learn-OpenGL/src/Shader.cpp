#include "Shader.h"
#include "Debugger.h"
#include <iostream> // input/output stream
#include <fstream>  // file stream
#include <sstream>  // string stream

Shader::Shader(const string& filepath) : m_FilePath(filepath)
{
	BasicShaders basicShaders = parseBasicShaders(filepath);
	m_RendererId = createProgram(basicShaders.vertexShaderSrc, basicShaders.fragmentShaderSrc);
}

Shader::~Shader()
{
	glCall(glDeleteProgram(m_RendererId));
}

void Shader::bind() const
{
	glCall(glUseProgram(m_RendererId));
}

void Shader::unbind() const
{
	glCall(glUseProgram(0));
}

void Shader::setUniform1i(const string& name, int v0) const
{
	glCall(glUniform1i(getUniformLocation(name), v0));
}

void Shader::setUniform1f(const string& name, float v0) const
{
	glCall(glUniform1f(getUniformLocation(name), v0));
}

void Shader::setUniform2f(const string& name, float v0, float v1) const
{
	glCall(glUniform2f(getUniformLocation(name), v0, v1));
}

void Shader::setUniform3f(const string& name, float v0, float v1, float v2) const
{
	glCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
}

void Shader::setUniform4f(const string& name, float v0, float v1, float v2, float v3) const
{
	glCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const string& name, const glm::mat4& matrix) const
{
	glCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

BasicShaders Shader::parseBasicShaders(const string& filepath) const
{
	// Open the input file
	ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType shaderType = ShaderType::NONE;
	stringstream ss[2];
	string line;
	while (getline(stream, line))
	{
		if (line.find("#shader") != string::npos)
		{
			if (line.find("vertex") != string::npos)
				shaderType = ShaderType::VERTEX;
			else if (line.find("fragment") != string::npos)
				shaderType = ShaderType::FRAGMENT;
		}
		else {
			if (shaderType != ShaderType::NONE)
				ss[(int)shaderType] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::createShader(unsigned int type, const string& source) const
{
	// Shader source code creation and compilation step
	glCall(unsigned int shaderId = glCreateShader(type));
	const char* const src = source.c_str();
	glCall(glShaderSource(shaderId, 1, &src, nullptr));
	glCall(glCompileShader(shaderId));

	// Error printing step
	int status;
	glCall(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status));

	if (status == GL_FALSE) // !status
	{
		// Getting the length of the error message
		int length;
		glCall(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length));

		// Getting the error message
		char* message = (char*)_malloca(length);
		glCall(glGetShaderInfoLog(shaderId, length, &length, message));

		// Logging the error message
		cout << "Failed to create " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << endl;
		cout << message << endl;

		glCall(glDeleteShader(shaderId));
		return 0;
	}

	return shaderId;
}

unsigned int Shader::createProgram(const string& vertexShaderSrc, const string& fragmentShaderSrc) const
{
	glCall(unsigned int programId = glCreateProgram()); // Create/Initialize the program
	unsigned int vs = createShader(GL_VERTEX_SHADER, vertexShaderSrc);
	unsigned int fs = createShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

	glCall(glAttachShader(programId, vs)); // Attach the vertex shader
	glCall(glAttachShader(programId, fs)); // Attach the fragment shader

	// Two regular steps at the end of creating any program
	glCall(glLinkProgram(programId));
	glCall(glValidateProgram(programId));

	// Delete the unused shaders
	glCall(glDetachShader(programId, vs));
	glCall(glDetachShader(programId, fs));
	glCall(glDeleteShader(vs));
	glCall(glDeleteShader(fs));
	return programId;
}

int Shader::getUniformLocation(const string& name) const
{
	if (m_UniformLocationCache.count(name))
		return m_UniformLocationCache.at(name);

	glCall(int location = glGetUniformLocation(m_RendererId, name.c_str()));
	if (location == -1)
		cout << "Warning: Uniform '" << name << "' doesn't exist!!" << endl;
	return location;
}
