#include "Window/Window.h"
#include "Settings/Settings.h"
#include "Debugging/Macros.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// Initialize the game's settings
	// In the future probably read this from a file
	Settings * MainSettings = new Settings(
		"Fantasi",	// Program Title
		512,		// Screen Width
		512,		// Screen Height
		false		// Is Full Screen
	);

	Window* MainWindow = new Window();
	MainWindow->Initialize(MainSettings);

	GLuint TextureHandle = g_OpenGLContext->CreateTexture(MainSettings->ScreenX, MainSettings->ScreenY);
	GLuint RenderHandle = g_OpenGLContext->CreateRenderProgram(TextureHandle);
	GLuint ComputeHandle = g_OpenGLContext->CreateComputeProgram(TextureHandle);

	Scene* MainScene = new Scene();
	MainScene->AddSphere(-1.0f, 0.0f, 10.0f, 1.0f);
	MainScene->AddSphere(1.0f,  2.0f, 10.0f, 0.8f);
	MainScene->AddSphere(0.0f, -2.0f, 10.0f, 0.6f);

	bool done = false;
	int frame = 0;
	while(!done)
	{
		// Handle Windows messages
		// If windows signals to end the application then exit out.
		if(MainWindow->HandleMessages() == WM_QUIT)
		{
			done = true;
		}
		else
		{
			MainScene->Tick(frame++);
			g_OpenGLContext->RayTrace(MainScene, ComputeHandle);
			g_OpenGLContext->Render(RenderHandle);
			GLenum e = glGetError();
			if (e != GL_NO_ERROR) {
				DEBUG_PRINT("OpenGL error: " << gluErrorString(e) << "(" << e << ")");
			}
		}

	}

	// Cleanup
	delete MainScene;
	delete MainWindow;
	delete MainSettings;
	return 0;
}
