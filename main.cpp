#include "main.h"
//#include "Render.h"
using namespace std;

int commandLine();
void printHelp();
void windowThread(void*);
void terminate();
void init();

Window wnd;
Builder build;
Render render;
Scene scene;

int main()
{
#if defined (DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	int res = commandLine();
	return res;
}

int commandLine()
{
	cout<<"Kinect information visualizer\nEnter '?' for help"<<endl;
	string cmd;
	do {
		cout<<"> ";
		cin>>cmd;
		if (cmd == "exit")
		{
			terminate();
			continue;
		}
		if (cmd == "start")
		{
			init();
			continue;
		}
		if (cmd == "load")
		{
			build.LoadImages(1);
			continue;
		}
		if (cmd == "build")
		{
			render.DisableDrawing(true);
			build.BuildModel(&scene);
			render.DisableDrawing(false);
			continue;
		}
		if (cmd == "?")
		{
			printHelp();
			continue;
		}
		cout<<"Unknown command. Type ? for help."<<endl;
	} while(cmd != "exit");
	return 0;
}

void printHelp()
{
	cout<<"\
	load: load test image\
	start: start working\
	exit: close program\n\
	?: show this page\n\
	"<<endl;
}

void windowThread(void*)
{
	MSG msg  = {0};
	wnd.Init((HINSTANCE)GetModuleHandle(NULL), SW_SHOW, &scene);
	render.Init(&wnd, &build, true);
	scene.Init(render.m_device);

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			scene.Update(0.0001f);
			render.OnPaint(&scene);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return wnd.WndProc(hWnd, message, wParam, lParam);
}

void terminate()
{
	wnd.Release();
}

void init()
{
	build.Init();
	_beginthread(windowThread, 0, NULL);
}