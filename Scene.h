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

	D3DXVECTOR3 defaultPos;
	D3DXVECTOR3 defaultTarget;
	D3DXVECTOR3 defaultUp;

	float m_XToTurn;
	float m_YToTurn;
public:
	Scene();
	IDirect3DDevice9* GetDevice();
	Camera *camera;
	VOID Update(float dt);
	VOID OnDestroy();
	std::vector<Object*>* GetObjects();
	BOOL Init(IDirect3DDevice9 *m_device);
	void TurnLeft(float angle);
	void TurnUp(float angle);
	void ResetCamera();
};
#endif