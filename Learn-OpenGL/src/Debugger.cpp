#include <iostream>
#include "Debugger.h"
using namespace std;

void glClearError()
{
	while (glGetError() != GL_NO_ERROR); // !glGetError()
}

bool glLogCall(const char* function, const char* file, int line)
{
	while (unsigned int error = glGetError())
	{
		cout << "[OpenGL Error!!] " << "(" << error << "): " << function << " " << file << " " << line << endl;
		return false;
	}
	return true;
}