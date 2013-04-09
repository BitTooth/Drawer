#include "window.h"

Window::Window()
{
	m_szClassName = "WinAPI";
	m_szTitle = "Visualizer";

	isLMBPressed = false;
	m_nOldMouseX = 0;
	m_nOldMouseY = 0;
}

UINT Window::GetWidth()
{
	RECT r;
	GetWindowRect(m_hWnd, &r);
	return r.right - r.left;
}

UINT Window::GetHeight()
{
	RECT r;
	GetWindowRect(m_hWnd, &r);
	return r.bottom - r.top;
}

HWND Window::GetWnd()
{
	return m_hWnd;
}

BOOL Window::InitApplication(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)(::WndProc),
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE-1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_szClassName;

	return RegisterClass(&wc);
}

BOOL Window::InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	m_hWnd = CreateWindow (
		m_szClassName,
		m_szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!m_hWnd) return (FALSE);
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);
	return (TRUE);
}

void Window::Init(HINSTANCE hInstance, int nCmdShow, Scene *scene)
{
	InitApplication(hInstance);
	InitInstance(hInstance, nCmdShow);
	m_pScene = scene;
}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		//Рисуем...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		isLMBPressed = true;
		m_nOldMouseX = LOWORD(lParam);// Count x
		m_nOldMouseY = HIWORD(lParam);// Count y
		break;
	case WM_MOUSEMOVE:
		if (isLMBPressed)
		{
			int newX = LOWORD(lParam);
			int newY = HIWORD(lParam);
			m_pScene->TurnLeft((float)(newX - m_nOldMouseX)*0.005f);
			m_nOldMouseX = newX;
			m_nOldMouseY = newY;
		}
		break;
	case WM_LBUTTONUP:
		isLMBPressed = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void Window::Release()
{
}