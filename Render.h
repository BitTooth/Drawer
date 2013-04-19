#ifndef __RENDER_H
#define __RENDER_H

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include "main.h"
//#include "d3dUtil.h"
////#include "object.h"
//#include "Scene.h"
//#include "main.h"
//#include "Builder.h"
//#include "Vertex.h"
//
//#include "Cube.h"

#define  MATRIX4 D3DXMATRIX

//Definitions for lost devices
#define DEVICE_UNKNOWN		0
#define DEVICE_LOST			1
#define DEVICE_NOT_RESET	2
#define DEVICE_ERROR		3
#define DEVICE_NORMAL		4


class Render
{
private:
	Window *m_window;
	Builder *m_builder;
	bool m_windowed;
	bool m_stopDrawing;

	IDirect3D9 *m_d3d9;
	D3DPRESENT_PARAMETERS m_param;
	BOOL AdditionalInitialization();

	D3DXHANDLE ghWVP;
	D3DXHANDLE ghWorldInverseTransform;
	D3DXHANDLE ghDiffuseMtl;
	D3DXHANDLE ghDiffuseLight;
	D3DXHANDLE ghLightVecW;
	D3DXHANDLE tech;
public:
	ID3DXEffect* mFX;

	VOID DisableDrawing(bool enabled);
	IDirect3DDevice9 *m_device;
	BOOL Init(Window *wnd, Builder *build, bool windowed);
	VOID OnPaint(Scene *scene);
	VOID OnDestroy();
	DWORD IsDeviceLost();
	BOOL OnReset();
};
#endif