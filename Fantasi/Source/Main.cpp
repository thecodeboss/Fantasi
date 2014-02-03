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
	Scene* MainScene = new Scene();

	Window* MainWindow = new Window();
	MainWindow->Initialize(MainSettings);
	MainWindow->AttachOnKeyDown(std::bind(&Scene::HandleOnKeyDown, MainScene, std::placeholders::_1));
	MainWindow->AttachOnKeyUp(std::bind(&Scene::HandleOnKeyUp, MainScene, std::placeholders::_1));

	GLuint TextureHandle = g_OpenGLContext->CreateTexture(MainSettings->ScreenX, MainSettings->ScreenY);
	GLuint RenderHandle = g_OpenGLContext->CreateRenderProgram(TextureHandle);
	GLuint ComputeHandle = g_OpenGLContext->CreateComputeProgram(TextureHandle);

	MainScene->ViewMatrix = mat4(1.0f);
	MainScene->ViewMatrix = glm::translate(MainScene->ViewMatrix, vec3(0.0f, 0.0f, -1.0f));
	MainScene->NumReflections = 2;

	MainScene->AddChild(Scene::S_BASICMATERIAL, new BasicMaterial(vec4(0.0f, 0.0f, 0.0f, 0.0f), vec4(0.07f, 0.07f, 0.07f, 1.0f), vec4(0.7f, 0.7f, 0.65f, 1.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f), 0.0f, 10.0f, 10.0f, 1.0f));

	// Polished Copper
	MainScene->AddChild(Scene::S_BASICMATERIAL, new BasicMaterial(vec4(0.0f, 0.0f, 0.0f, 0.0f), vec4(0.2295f, 0.08825f, 0.0275f, 1.0f), vec4(0.5508f, 0.2118f, 0.066f, 1.0f), vec4(0.580594f, 0.223257f, 0.0695701f, 1.0f), 51.2f, 2.43f, 6.6f, 0.0f));

	// Gold
	MainScene->AddChild(Scene::S_BASICMATERIAL, new BasicMaterial(vec4(0.0f, 0.0f, 0.0f, 0.0f), vec4(0.24725f, 0.1995f, 0.0745f, 1.0f), vec4(0.75164f, 0.60648f, 0.22648f, 1.0f), vec4(0.628281f, 0.555802f, 0.366065f, 1.0f), 51.2f, 0.47f, 10.0f, 0.0f));

	//for (int i=-5; i <= 5; i++)
	//{
	//	for (int j=-5; j <= 5; j++)
	//	{
	//		MainScene->AddChild(Scene::S_SPHERE, new Sphere(vec3(20.0f*i, -5.0f, 20.0f*j), 5.0f));
	//		MainScene->AddChild(Scene::S_OBJECT, new Object(0, (i+5)*11 + (j+5), 0, 2));
	//	}
	//}

	MainScene->AddChild(Scene::S_SPHERE, new Sphere(vec3(0.0f, 0.0f, 50.0f), 10.0f));
	MainScene->AddChild(Scene::S_OBJECT, new Object(0, 0, 0, 2));

	MainScene->AddChild(Scene::S_SPHERE, new Sphere(vec3(7.0f, -6.0f, 35.0f), 4.0f));
	MainScene->AddChild(Scene::S_OBJECT, new Object(0, 1, 0, 2));

	// Bottom
	MainScene->AddChild(Scene::S_TRIANGLE, new Triangle(vec3(-200.0f, -10.0f, -200.0f), vec3(200.0f, -10.0f, 200.0f), vec3(200.0f, -10.0f, -200.0f)));
	MainScene->AddChild(Scene::S_OBJECT, new Object(1, 0, 0, 1));
	MainScene->AddChild(Scene::S_TRIANGLE, new Triangle(vec3(-200.0f, -10.0f, -200.0f), vec3(-200.0f, -10.0f, 200.0f), vec3(200.0f, -10.0f, 200.0f)));
	MainScene->AddChild(Scene::S_OBJECT, new Object(1, 1, 0, 1));

	// Top
	MainScene->AddChild(Scene::S_TRIANGLE, new Triangle(vec3(-200.0f, 50.0f, -200.0f), vec3(200.0f, 50.0f, -200.0f), vec3(200.0f, 50.0f, 200.0f)));
	MainScene->AddChild(Scene::S_OBJECT, new Object(1, 2, 0, 1));
	MainScene->AddChild(Scene::S_TRIANGLE, new Triangle(vec3(-200.0f, 50.0f, -200.0f), vec3(200.0f, 50.0f, 200.0f), vec3(-200.0f, 50.0f, 200.0f)));
	MainScene->AddChild(Scene::S_OBJECT, new Object(1, 3, 0, 1));

	// Back
	MainScene->AddChild(Scene::S_TRIANGLE, new Triangle(vec3(-200.0f, -10.0f, 200.0f), vec3(200.0f, 50.0f, 200.0f), vec3(200.0f, -10.0f, 200.0f)));
	MainScene->AddChild(Scene::S_OBJECT, new Object(1, 4, 0, 1));
	MainScene->AddChild(Scene::S_TRIANGLE, new Triangle(vec3(-200.0f, -10.0f, 200.0f), vec3(-200.0f, 50.0f, 200.0f), vec3(200.0f, 50.0f, 200.0f)));
	MainScene->AddChild(Scene::S_OBJECT, new Object(1, 5, 0, 1));

	// Front
	MainScene->AddChild(Scene::S_TRIANGLE, new Triangle(vec3(-200.0f, -10.0f, -200.0f), vec3(200.0f, -10.0f, -200.0f), vec3(200.0f, 50.0f, -200.0f)));
	MainScene->AddChild(Scene::S_OBJECT, new Object(1, 6, 0, 1));
	MainScene->AddChild(Scene::S_TRIANGLE, new Triangle(vec3(-200.0f, -10.0f, -200.0f), vec3(200.0f, 50.0f, -200.0f), vec3(-200.0f, 50.0f, -200.0f)));
	MainScene->AddChild(Scene::S_OBJECT, new Object(1, 7, 0, 1));

	// Left
	MainScene->AddChild(Scene::S_TRIANGLE, new Triangle(vec3(-200.0f, -10.0f, -200.0f), vec3(-200.0f, 50.0f, 200.0f), vec3(-200.0f, -10.0f, 200.0f)));
	MainScene->AddChild(Scene::S_OBJECT, new Object(1, 8, 0, 1));
	MainScene->AddChild(Scene::S_TRIANGLE, new Triangle(vec3(-200.0f, -10.0f, -200.0f), vec3(-200.0f, 50.0f, -200.0f), vec3(-200.0f, 50.0f, 200.0f)));
	MainScene->AddChild(Scene::S_OBJECT, new Object(1, 9, 0, 1));

	// Right
	MainScene->AddChild(Scene::S_TRIANGLE, new Triangle(vec3(200.0f, -10.0f, -200.0f), vec3(200.0f, -10.0f, 200.0f), vec3(200.0f, 50.0f, 200.0f)));
	MainScene->AddChild(Scene::S_OBJECT, new Object(1, 10, 0, 1));
	MainScene->AddChild(Scene::S_TRIANGLE, new Triangle(vec3(200.0f, -10.0f, -200.0f), vec3(200.0f, 50.0f, 200.0f), vec3(200.0f, 50.0f, -200.0f)));
	MainScene->AddChild(Scene::S_OBJECT, new Object(1, 11, 0, 1));

	float x_min = -10.0f;
	float x_max = -9.0f;
	float x_inc = 1.5f;
	float z_min = 9.0f;
	float z_max = 10.0f;
	float z_inc = 1.5f;

	float NumLights = (x_max - x_min)*(z_max - z_min) / (x_inc*z_inc);
	float sf = 1.0f/NumLights;
	float Intensity = 0.4f;

	for (float i=x_min; i <= x_max; i+=x_inc)
	{
		for (float j=z_min; j <= z_max; j+=z_inc)
		{
			MainScene->AddChild(Scene::S_POINTLIGHT, new PointLight(vec3(i, 20.0f, j), Intensity*sf*vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		}
	}

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
