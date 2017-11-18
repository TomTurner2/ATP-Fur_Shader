#include <Windows.h>
#include "Window.h"
#include <winuser.h>
#include "AntTweakBar.h"

LRESULT CALLBACK WinProc(HWND _handle, UINT _msg, WPARAM _wparam, LPARAM _lparam)
{
	if(_msg == WM_DESTROY || _msg == WM_CLOSE)
	{
		PostQuitMessage(0);// Check for quit
		return 0;
	}

	if (TwEventWin(_handle, _msg, _wparam, _lparam)) // send event message to AntTweakBar
		return 0; // event has been handled by


	return DefWindowProc(_handle, _msg, _wparam, _lparam);
}


Window::Window(std::string _window_title, unsigned int _width, unsigned int _height,
	int _initial_x_pos, int _initial_y_pos) : m_width(_width), m_height (_height)
{
	// Define window Style
	WNDCLASS wc = { 0 };
	
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WinProc;// Window event processing function
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = TEXT(W_CLASS_NAME.c_str());// Set window class name

	RegisterClass(&wc);

	m_handle = CreateWindow(wc.lpszClassName, TEXT(_window_title.c_str()), WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		_initial_x_pos, _initial_y_pos, _width, _height, nullptr, nullptr, nullptr, nullptr);
}

float Window::GetAspectRatio() const
{
	float aspect_ratio = m_width / m_height;
	return aspect_ratio;
}


#pragma region Getters
HWND Window::GetHandle() const
{
	return m_handle;
}


unsigned int Window::GetWindowWidth() const
{
	return m_width;
}


unsigned int Window::GetWindowHeight() const
{
	return m_height;
}
#pragma endregion