#ifndef Shader_h__
#define Shader_h__

#include <string>
#include "../Debugging/Macros.h"
#include "GL.h"

class Shader
{
	std::string ReadShaderFile(const char *ShaderFile);
public:
	Shader();

	// ShaderType = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER }
	GLuint CreateAndAttach(const char *ShaderFile, GLuint ShaderType, GLuint ProgramHandle);
};

#endif // Shader_h__
