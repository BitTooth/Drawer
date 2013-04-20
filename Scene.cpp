#include "scene.h"

//extern Input *gInput;

Scene::Scene()
{
	camera = new Camera();
	defaultPos = D3DXVECTOR3(0.f, 2500.0f, 2500.0f);
	defaultTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	defaultUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	ResetCamera();
}

void Scene::ResetCamera()
{
	camera->m_pos		= defaultPos;
	camera->m_target	= defaultTarget;
	camera->m_up		= defaultUp;
}

VOID Scene::TurnLeft(float angle)
{
	m_XToTurn += angle;
}

VOID Scene::TurnUp(float angle)
{
	m_YToTurn += angle;
}

VOID Scene::Update(float dt)
{
	static float p = 0;
	
	// std::vector<Object*>::iterator it;
	// for (it = m_objects.begin(); it != m_objects.end(); ++it)
	// {
	// 	//Do any work with objects
	// }

	D3DXMATRIX rot;
	// D3DXVECTOR3 vec(0.f, 1.f, 0.f);
	D3DXMatrixRotationAxis(&rot, &(camera->m_up), m_XToTurn);

	D3DXVECTOR4 res;
	D3DXVec3Transform(&res, &(camera->m_pos), &rot);

	D3DXMatrixIdentity(&rot);
	D3DXVECTOR3 axis;
	D3DXVECTOR3 eye(res.x, res.y, res.z);

	D3DXVec3Cross(&axis, &eye, &camera->m_up);
	D3DXVec3Normalize(&axis, &axis);

	m_YToTurn *= -axis.x / abs(axis.x);

	D3DXMatrixRotationAxis(&rot, &axis, m_YToTurn);
	D3DXVec3Transform(&res, &(camera->m_up), &rot);

	camera->m_up.x = res.x;
	camera->m_up.y = res.y;
	camera->m_up.z = res.z;

	D3DXVec3Transform(&res, &eye, &rot);

	camera->m_pos.x = res.x;
	camera->m_pos.y = res.y;
	camera->m_pos.z = res.z;
	m_XToTurn = 0.f;
	m_YToTurn = 0.f;
}

VOID Scene::OnDestroy()
{
	std::vector<Object*>::iterator it;
	for (it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->Release();
	}
}

std::vector<Object*>* Scene::GetObjects()
{
	return &m_objects;
}

IDirect3DDevice9* Scene::GetDevice()
{
	return m_pDevice;
}

BOOL Scene::Init(IDirect3DDevice9 *m_device)
{
	m_XToTurn = 0.f;
	m_pDevice = m_device;

	m_objects.push_back(new Cube(m_device));
	m_objects.push_back(new Cube(m_device));
	m_objects.push_back(new Cube(m_device));
	m_objects.push_back(new Cube(m_device));
	m_objects.push_back(new Cube(m_device));

	m_objects[0]->SetPos(D3DXVECTOR3(2.f, 2.f, 2.f));		//
	m_objects[0]->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));		// KAMERA
	m_objects[0]->SetRotation(D3DXVECTOR3(0.f, 0.f, 0.f));  // 
	
	m_objects[1]->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));		// 
	m_objects[1]->SetScale(D3DXVECTOR3(80.f, 0.1f, 80.f));	// LABEL
	m_objects[1]->SetRotation(D3DXVECTOR3(0.f, 0.f, 0.f));  // 
	
	m_objects[2]->SetPos(D3DXVECTOR3(50.f, 0.f, 0.f));		// 
	m_objects[2]->SetScale(D3DXVECTOR3(100.f, 1.f, 1.f));	//	X AXIS
	m_objects[2]->SetRotation(D3DXVECTOR3(0.f, 0.f, 0.f));	// 
	
	m_objects[3]->SetPos(D3DXVECTOR3(0.f, 50.f, 0.f));		//  
	m_objects[3]->SetScale(D3DXVECTOR3(1.f, 100.f, 1.f));	//  Y AXIS
	m_objects[3]->SetRotation(D3DXVECTOR3(0.f, 0.f, 0.f));	//  
	
	m_objects[4]->SetPos(D3DXVECTOR3(0.f, 0.f, 50.f));		//  
	m_objects[4]->SetScale(D3DXVECTOR3(1.f, 1.f, 100.f));	//  Z AXIS
	m_objects[4]->SetRotation(D3DXVECTOR3(0.f, 0.f, 0.f));	//

	// float triangle[] = 
	// {
	// 	-1.0f, 0.0f, -1.0f, 0.f, -1.f, 0.f,
	// 	-1.0f, 0.0f, +1.0f, 0.f, -1.f, 0.f,
	// 	+1.0f, 0.0f, +1.0f, 0.f, -1.f, 0.f
	// };
	// 
	// DWORD triagIndex[] = { 0, 1, 2 };
	// 
	// Object *triagMesh = new Mesh(m_device, triangle, triagIndex, 3, 3);
	// triagMesh->SetPos(D3DXVECTOR3(0.f, 50.f, 0.f));
	// triagMesh->SetScale(D3DXVECTOR3(100.f, 1.f, 100.f));
	// triagMesh->SetRotation(D3DXVECTOR3(0.0f, 90.0f, 0.0f));
	// 
	// m_objects.push_back(triagMesh);
	return TRUE;
}