#include "OpenGLContext.h"
#include "../Debugging/Macros.h"

void checkErrors()
{
	GLenum e = glGetError();
	if (e != GL_NO_ERROR) {
		DEBUG_PRINT("OpenGL error: " << gluErrorString(e) << "(" << e << ")");
	}
}

OpenGLContext::OpenGLContext()
{

}

bool OpenGLContext::CreateContext(HWND WindowIdentifier, Settings* settings) {
	m_WindowIdentifier = WindowIdentifier; // Set the WindowsIdentifier for our window
	settings = settings;

	m_DeviceContext = GetDC(WindowIdentifier); // Get the device context for our window

	PIXELFORMATDESCRIPTOR PixelDescription; // Create a new PIXELFORMATDESCRIPTOR (PFD)
	memset(&PixelDescription, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD
	PixelDescription.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class
	PixelDescription.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window
	PixelDescription.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
	PixelDescription.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)
	PixelDescription.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)
	PixelDescription.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD

	int pixelFormat = ChoosePixelFormat(m_DeviceContext, &PixelDescription);
	if (pixelFormat == 0) return false;

	BOOL Result = SetPixelFormat(m_DeviceContext, pixelFormat, &PixelDescription);
	if (!Result) return false;

	HGLRC TempOpenGLContext = wglCreateContext(m_DeviceContext); // Create an OpenGL 2.1 context for our device context
	wglMakeCurrent(m_DeviceContext, TempOpenGLContext); // Make the OpenGL 2.1 context current and active

	GLenum error = glewInit(); // Enable GLEW
	if (error != GLEW_OK) return false; // If GLEW fails

	// Need at least OpenGL 4.3 for OpenGL Compute Shader support
	int attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
		0
	};

	// Get function pointers for OpenGL extensions

	m_RenderingContext = wglCreateContextAttribsARB(m_DeviceContext, nullptr, attributes); // Create the OpenGL 4.x context based on the given attributes
	wglMakeCurrent(nullptr, nullptr); // Remove the temporary context from being active
	wglDeleteContext(TempOpenGLContext); // Delete the temporary OpenGL 2.1 context
	wglMakeCurrent(m_DeviceContext, m_RenderingContext); // Make our OpenGL 4.x context current

	// Finding the compute shader extension
	int ExtensionCount;
	glGetIntegerv(GL_NUM_EXTENSIONS, &ExtensionCount);

	bool Found = false;
	for (int i = 0; i < ExtensionCount; ++i)
	{
		if (!strcmp((const char*)glGetStringi(GL_EXTENSIONS, i), "GL_ARB_compute_shader")) {
			DEBUG_PRINT("Extension \"GL_ARB_compute_shader\" found\n");
			Found = true;
			break;
		}
	}

	if (!Found) {
		DEBUG_PRINT("Extension \"GL_ARB_compute_shader\" not found\n");
		return false;
	}

	glViewport(0, 0, settings->ScreenX, settings->ScreenY);

	checkErrors();

	return true; // We have successfully created a context, return true
}

GLuint OpenGLContext::CreateTexture(GLsizei width, GLsizei height) {
	// We create a single float channel 512^2 texture
	GLuint texHandle;
	glGenTextures(1, &texHandle);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindImageTexture(0, texHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	return texHandle;
}

GLuint OpenGLContext::CreateRenderProgram(GLuint texHandle) {
	GLuint progHandle = glCreateProgram();

	Shader* VShader = new Shader();
	GLuint vp = VShader->CreateAndAttach("Source\\Shaders\\Shader.vert", GL_VERTEX_SHADER, progHandle);

	Shader* FShader = new Shader();
	GLuint fp = FShader->CreateAndAttach("Source\\Shaders\\Shader.frag", GL_FRAGMENT_SHADER, progHandle);

	glBindFragDataLocation(progHandle, 0, "color");
	glLinkProgram(progHandle);

	int rvalue;
	glGetProgramiv(progHandle, GL_LINK_STATUS, &rvalue);
	if (!rvalue) {
		DEBUG_PRINT("Error in linking sp\n");
		return 0;
	}   

	glUseProgram(progHandle);
	glUniform1i(glGetUniformLocation(progHandle, "srcTex"), 0);

	GLuint vertArray;
	glGenVertexArrays(1, &vertArray);
	glBindVertexArray(vertArray);

	GLuint posBuf;
	glGenBuffers(1, &posBuf);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	float data[] = {
		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, data, GL_STREAM_DRAW);
	GLint posPtr = glGetAttribLocation(progHandle, "pos");
	glVertexAttribPointer(posPtr, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posPtr);

	return progHandle;
}

GLuint OpenGLContext::CreateComputeProgram(GLuint texHandle) {
	// Creating the compute shader, and the program object containing the shader
	GLuint progHandle = glCreateProgram();
	Shader* shader = new Shader();
	GLuint cs = shader->CreateAndAttach("Source\\Shaders\\Shader.comp", GL_COMPUTE_SHADER, progHandle);

	int rvalue;
	glLinkProgram(progHandle);
	glGetProgramiv(progHandle, GL_LINK_STATUS, &rvalue);
	if (!rvalue) {
		DEBUG_PRINT("Error in linking compute shader program\n");
		GLchar log[10240];
		GLsizei length;
		glGetProgramInfoLog(progHandle, 10239, &length, log);
		DEBUG_PRINT("Linker log: " << log);
		return 0;
	}   
	glUseProgram(progHandle);

	glGenBuffers(1, &SpheresBufferID);
	glGenBuffers(1, &PointLightsBufferID);

	glUniform1i(glGetUniformLocation(progHandle, "destTex"), 0);

	return progHandle;
}

void OpenGLContext::RayTrace(Scene* scene, GLuint ComputeHandle)
{
	glUseProgram(ComputeHandle);


	size_t NumSpheres = scene->GetNumSpheres();
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SpheresBufferID);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SpheresBufferID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NumSpheres*sizeof(Sphere), NULL, GL_STATIC_DRAW);
	struct Sphere* spheres = (struct Sphere*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NumSpheres*sizeof(Sphere), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for (size_t i=0; i < NumSpheres; i++)
	{
		spheres[i] = scene->GetSphereAt(static_cast<unsigned>(i));
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glUniform1ui(glGetUniformLocation(ComputeHandle, "NumSpheres"), static_cast<GLuint>(NumSpheres));


	size_t NumPointLights = scene->GetNumPointLights();
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, PointLightsBufferID);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, PointLightsBufferID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NumPointLights*sizeof(PointLight), NULL, GL_STATIC_DRAW);
	struct PointLight* pointlights = (struct PointLight*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NumPointLights*sizeof(PointLight), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for (size_t i=0; i < NumPointLights; i++)
	{
		pointlights[i] = scene->GetPointLightAt(static_cast<unsigned>(i));
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glUniform1ui(glGetUniformLocation(ComputeHandle, "NumPointLights"), static_cast<GLuint>(NumPointLights));

	glDispatchCompute(512/16, 512/16, 1); // 512^2 threads in blocks of 16^2
}

void OpenGLContext::Render(GLuint RenderHandle)
{
	glUseProgram(RenderHandle);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	SwapBuffers(m_DeviceContext);
}
