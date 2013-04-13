#ifndef __SCENE_H
#define __SCENE_H

//#include <vector>
////#include "object.h"
////#include "Camera.h"
//#include <fstream>
//#include <windows.h>
////#include "plane.h"
//#include "cube.h"
//#include "Vertex.h"
//// #include "Sphere.h"
//// #include "SceneObject.h"
//// #include "MaterialBrowser.h"
//// #include "EffectsBrowser.h"
#include "main.h"
struct Camera 
{
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_target;
	D3DXVECTOR3 m_up;
};

class Scene
{
private:
	std::vector<Object*> m_objects;

	float m_XToTurn;
public:
	Scene();
	Camera *camera;
	VOID Update(float dt);
	VOID OnDestroy();
	std::vector<Object*>* GetObjects();
	BOOL Init(IDirect3DDevice9 *m_device);
	void TurnLeft(float angle);
	void TurnRight(float angle);
};
#endif