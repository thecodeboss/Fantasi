#include "Window.h"

#pragma region Global Variables
Window* g_MainWindow;
OpenGLContext* g_OpenGLContext;
#pragma endregion

#pragma region Windows Callbacks
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
	case WM_SIZE:
		{
			return 0;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	default:
		{
			return g_MainWindow->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

LRESULT CALLBACK Window::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
	case WM_KEYDOWN:
		{
			OnKeyDownBinding(wparam);
			return 0;
		}
	case WM_KEYUP:
		{
			OnKeyUpBinding(wparam);
			return 0;
		}
	default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}
#pragma endregion

bool Window::Initialize(Settings* settings)
{
	// Set an external pointer to this object.
	// This will become useful for callbacks.
	g_MainWindow = this;

	// Get the instance of this window.
	m_hinstance = GetModuleHandle(nullptr);

	// Handle application settings
	settings = settings;

	// Setup the window with default settings for now.
	WNDCLASSEX WindowContext;
	WindowContext.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WindowContext.lpfnWndProc = WndProc;
	WindowContext.cbClsExtra = 0;
	WindowContext.cbWndExtra = 0;
	WindowContext.hInstance = m_hinstance;
	WindowContext.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	WindowContext.hIconSm = WindowContext.hIcon;
	WindowContext.hCursor = LoadCursor(nullptr, IDC_ARROW);
	WindowContext.hbrBackground = nullptr;
	WindowContext.lpszMenuName = nullptr;
	WindowContext.lpszClassName = settings->ProgramTitle;
	WindowContext.cbSize = sizeof(WNDCLASSEX);

	// Register the window
	RegisterClassEx(&WindowContext);

	int posX, posY;
	DWORD WindowExStyle, WindowStyle;

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(settings->bFullScreen)
	{
		// Determine the resolution of the user's screen.
		settings->ScreenX  = GetSystemMetrics(SM_CXSCREEN);
		settings->ScreenY = GetSystemMetrics(SM_CYSCREEN);

		// If full screen set the screen to maximum size of the users desktop and 32bit.
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)settings->ScreenX;
		dmScreenSettings.dmPelsHeight = (unsigned long)settings->ScreenY;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;

		// Set window style so there is no edge
		WindowExStyle = WS_EX_APPWINDOW;
		WindowStyle = WS_EX_TOPMOST | WS_POPUP;
	}
	else
	{
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - settings->ScreenX)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - settings->ScreenY) / 2;

		// Set window style to have an edge
		WindowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		WindowStyle = WS_OVERLAPPEDWINDOW;
	}

	// Creating the Window handle could resize the screen, which in turn requires
	// an OpenGL context.  So we create it right before the Window.
	g_OpenGLContext = new OpenGLContext();

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WindowExStyle, settings->ProgramTitle, settings->ProgramTitle, WindowStyle, 
		posX, posY, settings->ScreenX + 16, settings->ScreenY + 39, nullptr, nullptr, m_hinstance, nullptr);

	g_OpenGLContext->CreateContext(m_hwnd, settings); //Create our OpenGL context on the given window we just created

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	// ShowCursor(false);

	return true;
}

bool Window::Shutdown()
{
	delete g_OpenGLContext;
	return true;
}

unsigned Window::HandleMessages()
{
	// Handle the windows messages.
	if(PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}

	return m_msg.message;
}

void Window::AttachOnKeyDown( std::function<void(WPARAM)> f )
{
	OnKeyDownBinding = f;
}

void Window::AttachOnKeyUp( std::function<void(WPARAM)> f )
{
	OnKeyUpBinding = f;
}
