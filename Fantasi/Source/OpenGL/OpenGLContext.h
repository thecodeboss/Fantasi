#ifndef OpenGLContext_h__
#define OpenGLContext_h__

#include <windows.h>
#include "GL.h"
#include "../Settings/Settings.h"
#include "Shader.h"
#include "../Scene.h"

class OpenGLContext
{
public:
	OpenGLContext();
	bool CreateContext(HWND WindowIdentifier, Settings* settings);
	GLuint CreateTexture(GLsizei width = 512, GLsizei height = 512);
	GLuint CreateRenderProgram(GLuint texHandle);
	GLuint CreateComputeProgram(GLuint texHandle);
	void RayTrace(Scene* scene, GLuint ComputeHandle);
	void Render(GLuint ProgramHandle);
	void* MapBuffer(GLuint ComputeHandle, GLuint BufferID, GLuint LayoutID, size_t BufferSize, size_t ElementSize);
private:
	HGLRC m_RenderingContext;
	HDC m_DeviceContext;
	HWND m_WindowIdentifier;
	Settings* settings;

	GLuint BufferIDs[Scene::S_COUNT];
};

#endif // OpenGLContext_h__
