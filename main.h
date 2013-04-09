#include <Windows.h>
#include <iostream>
#include <string>
#include <process.h>
#include <vector>

class Scene;
class Builder;
class Window;
class Render;
class Cube;
class Vertex;

#include "d3dUtil.h"
#include "ARtoolkit.h"
#include "Builder.h"
#include "Cube.h"
#include "Render.h"
#include "Scene.h"
#include "Vertex.h"
#include "window.h"



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);