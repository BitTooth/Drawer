#include "Cube.h"

Cube::Cube(IDirect3DDevice9 *m_device)
{
	HR(D3DXCreateBox(m_device, 1.0f, 1.0f, 1.0f, &mesh, NULL));
	pos.x = 0.f;
	pos.y = 0.f;
	pos.z = 0.f;
}

D3DXVECTOR3 Cube::GetPos()
{
	return pos;
}

D3DXVECTOR3 Cube::GetScale()
{
	return scale;
}
D3DXVECTOR3 Cube::GetRotation()
{
	return rotation;
}
void Cube::SetPos(D3DXVECTOR3 vec)
{
	pos = vec;
	FormTransMatrix();

}
void Cube::SetScale(D3DXVECTOR3 vec)
{
	scale = vec;
	FormTransMatrix();
}
void Cube::SetRotation(D3DXVECTOR3 vec)
{
	rotation = vec;
	FormTransMatrix();
}

void Cube::FormTransMatrix()
{
	D3DXMATRIX transl;
	D3DXMATRIX rot;
	D3DXMATRIX scal;

	D3DXMatrixIdentity(&transl);
	D3DXMatrixIdentity(&rot);
	D3DXMatrixIdentity(&scal);

	D3DXMatrixTranslation(&transl, pos.x, pos.y, pos.z);
	D3DXMatrixScaling(&scal, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, rotation.x * 3.14f/180.f, rotation.y * 3.14f/180.f, rotation.z * 3.14f/180.f);

	trans = scal*rot*transl;
}

D3DXMATRIX Cube::GetTransMatrix()
{
	return trans;
}

void Cube::SetTransMatrix(D3DXMATRIX matrix)
{	
	trans = matrix;
}

BOOL Cube::Draw()
{
	HR(mesh->DrawSubset(0));
	return TRUE;
}

VOID Cube::Release()
{
	mesh->Release();
}