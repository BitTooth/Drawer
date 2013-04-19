#ifndef __SCENE_H
#define __SCENE_H

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
	IDirect3DDevice9 *m_pDevice;

	float m_XToTurn;
public:
	Scene();
	IDirect3DDevice9* GetDevice();
	Camera *camera;
	VOID Update(float dt);
	VOID OnDestroy();
	std::vector<Object*>* GetObjects();
	BOOL Init(IDirect3DDevice9 *m_device);
	void TurnLeft(float angle);
	void TurnRight(float angle);
};
#endif