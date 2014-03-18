#ifndef Window_h__
#define Window_h__

#include "../Debugging/Macros.h"
#include <windows.h>
#include <functional>
#include "../OpenGL/OpenGLContext.h"
#include "../Settings/Settings.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

class Window
{

	HINSTANCE m_hinstance;
	HWND m_hwnd;
	MSG m_msg;
	Settings* settings;

	std::function<void(WPARAM)> OnKeyDownBinding;
	std::function<void(WPARAM)> OnKeyUpBinding;

public:

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
	bool Initialize(Settings* settings);
	bool Shutdown();
	unsigned HandleMessages();

	void AttachOnKeyDown(std::function<void(WPARAM)> f);
	void AttachOnKeyUp(std::function<void(WPARAM)> f);
};

extern Window* g_MainWindow;
extern OpenGLContext* g_OpenGLContext;

#endif // Window_h__
