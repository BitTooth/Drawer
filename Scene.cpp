#include "scene.h"

//extern Input *gInput;

Scene::Scene()
{
	camera = new Camera();
	D3DXVECTOR3 camPos(450.0f, 450.0f, 450.0f);
	D3DXVECTOR3 camTarget(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 camUp(0.0f, 1.0f, 0.0f);

	camera->m_pos = camPos;
	camera->m_target = camTarget;
	camera->m_up = camUp;

	/*if (!DeclareColorVertex())
	{
	char mes[256];
	sprintf_s(mes, 256, "Failed declare ColorVertex in file %d line %d", __FILE__, __LINE__);
	MessageBox(NULL, mes, "Error", MB_OK | MB_ICONERROR);
	}*/
}

VOID Scene::TurnLeft(float angle)
{
	m_XToTurn += angle;
}


VOID Scene::Update(float dt)
{
	static float p = 0;
	
	std::vector<Object*>::iterator it;
	for (it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		//Do any work with objects
	}

	D3DXMATRIX rot;
	D3DXVECTOR3 vec(0.f, 1.f, 0.f);
	D3DXMatrixRotationAxis(&rot, &vec, m_XToTurn);
	D3DXVECTOR4 res;
	D3DXVec3Transform(&res, &(camera->m_pos), &rot);
	camera->m_pos.x = res.x;
	camera->m_pos.y = res.y;
	camera->m_pos.z = res.z;
	m_XToTurn = 0.f;

	// D3DXVECTOR3 rot = m_objects[0]->GetRotation();
	// rot.z += 10.f * dt;
	// rot.x += 5.f *dt;
	// m_objects[0]->SetRotation(rot);
	// 
	// rot = m_objects[0]->GetScale();
	// rot.x -= rot.x*0.00005f;
	// m_objects[0]->SetScale(rot);

	/*if (gInput->keyLeft)
	{
	p += 5.0f * dt;
	D3DXVECTOR3 camPos(20.0f*cos(p), 20.0f*sin(p), camera->m_pos.z);
	camera->m_pos = camPos;
	}
	if (gInput->keyRight)
	{
	p -= 5.0f * dt;
	D3DXVECTOR3 camPos(20.0f*cos(p), 20.0f*sin(p), camera->m_pos.z);
	camera->m_pos = camPos;
	}

	if (gInput->keyUp)
	{
	D3DXVECTOR3 camPos(camera->m_pos.x, camera->m_pos.y, camera->m_pos.z + 10.0f*dt);
	camera->m_pos = camPos;
	}
	if (gInput->keyDown)
	{
	D3DXVECTOR3 camPos(camera->m_pos.x, camera->m_pos.y, camera->m_pos.z - 10.0f*dt);
	camera->m_pos = camPos;
	}*/
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

BOOL Scene::Init(IDirect3DDevice9 *m_device)
{
	m_XToTurn = 0.f;

	m_objects.push_back(new Cube(m_device));
	m_objects.push_back(new Cube(m_device));
	m_objects.push_back(new Cube(m_device));
	m_objects.push_back(new Cube(m_device));
	m_objects.push_back(new Cube(m_device));

	m_objects[0]->SetPos(D3DXVECTOR3(2.f, 2.f, 2.f));		//
	m_objects[0]->SetScale(D3DXVECTOR3(50.f, 50.f, 70.f));	// KAMERA
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

	float triangle[] = 
	{
		-1.0f, 0.0f, -1.0f, 0.f, -1.f, 0.f,
		-1.0f, 0.0f, +1.0f, 0.f, -1.f, 0.f,
		+1.0f, 0.0f, +1.0f, 0.f, -1.f, 0.f
	};

	DWORD triagIndex[] = { 0, 1, 2 };

	Object *triagMesh = new Mesh(m_device, triangle, triagIndex, 3, 3);
	triagMesh->SetPos(D3DXVECTOR3(0.f, 50.f, 0.f));
	triagMesh->SetScale(D3DXVECTOR3(100.f, 1.f, 100.f));
	triagMesh->SetRotation(D3DXVECTOR3(0.0f, 90.0f, 0.0f));

	m_objects.push_back(triagMesh);
	return TRUE;
}