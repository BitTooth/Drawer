#ifndef __WINDOW_H
#define __WINDOW_H

#include "main.h"

class Window
{
private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	LPCSTR m_szClassName;
	LPCSTR m_szTitle;

	Scene *m_pScene;

	BOOL InitApplication(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

	bool isLMBPressed;
	int m_nOldMouseX;
	int m_nOldMouseY;
public:
	Window();
	void Init(HINSTANCE hInstance, int nCmdShow, Scene *scene);
	void Release();
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	UINT GetWidth();
	UINT GetHeight();
	HWND GetWnd();
};

#endif