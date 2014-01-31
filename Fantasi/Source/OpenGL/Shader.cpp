#include "Shader.h"
#include "../Debugging/Macros.h"
#include <fstream>
using namespace std;

Shader::Shader()
{

}

GLuint Shader::CreateAndAttach(const char *ShaderFile, GLuint ShaderType, GLuint ProgramHandle)
{
	GLuint ShaderHandle = glCreateShader(ShaderType);
	string ShaderSource = ReadShaderFile(ShaderFile);
	const char * ShaderCStr = ShaderSource.c_str();

	glShaderSource(ShaderHandle, 1, &ShaderCStr, NULL);
	glCompileShader(ShaderHandle);
	int rvalue;
	glGetShaderiv(ShaderHandle, GL_COMPILE_STATUS, &rvalue);
	if (!rvalue) {
		DEBUG_PRINT("Error in compiling Shader: \n" << ShaderFile);
		GLchar log[10240];
		GLsizei length;
		glGetShaderInfoLog(ShaderHandle, 10239, &length, log);
		DEBUG_PRINT("\nCompiler log: " << log);
		return 0;
	}
	glAttachShader(ProgramHandle, ShaderHandle);

	return ShaderHandle;
}

string Shader::ReadShaderFile( const char *ShaderFile )
{
	ifstream File(ShaderFile);
	if (!File.is_open())
	{
		DEBUG_PRINT("Failed to open file " << ShaderFile)
	}
	
	string str;

	File.seekg(0, ios::end);
	str.reserve(File.tellg());
	File.seekg(0, ios::beg);

	str.assign((istreambuf_iterator<char>(File)), istreambuf_iterator<char>());

	return str;
}
