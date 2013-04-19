#include "Mesh.h"



Mesh::Mesh(LPDIRECT3DDEVICE9 pD3DDevice, float *vertecies, DWORD *indeces, int numVertex, int numIndex)
{

	static D3DVERTEXELEMENT9 VertexPNDeclArr[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		D3DDECL_END()
	};

	DWORD facesCount, vertsCount;
	DWORD createMeshOpt = 0;
	createMeshOpt |= D3DXMESH_32BIT;
	createMeshOpt |= D3DXMESH_SYSTEMMEM;


	IDirect3DVertexBuffer9 *vertex;
	IDirect3DIndexBuffer9  *index;
	DWORD *attributes;
	HR(D3DXCreateMesh(numIndex, numVertex, createMeshOpt, VertexPNDeclArr,  pD3DDevice, &mesh));

	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	mesh->GetDeclaration(decl);

	mesh->LockVertexBuffer(NULL, (void**)&vertex);
	int a = sizeof(*decl);
	memcpy(vertex, vertecies, numVertex * 6 * sizeof(float));
	mesh->UnlockVertexBuffer();

	HR(mesh->LockIndexBuffer(NULL, (void**)&index));
	memcpy(index, indeces, numIndex * sizeof(DWORD));
	HR(mesh->UnlockIndexBuffer());

	mesh->LockAttributeBuffer(NULL, &attributes);
	memset(attributes, 0, numVertex * sizeof(DWORD));
	mesh->UnlockAttributeBuffer();



	pos.x = 0.f;
	pos.y = 0.f;
	pos.z = 0.f;
}

D3DXVECTOR3 Mesh::GetPos()
{
	return pos;
}

D3DXVECTOR3 Mesh::GetScale()
{
	return scale;
}
D3DXVECTOR3 Mesh::GetRotation()
{
	return rotation;
}
void Mesh::SetPos(D3DXVECTOR3 vec)
{
	pos = vec;
	FormTransMatrix();

}
void Mesh::SetScale(D3DXVECTOR3 vec)
{
	scale = vec;
	FormTransMatrix();
}
void Mesh::SetRotation(D3DXVECTOR3 vec)
{
	rotation = vec;
	FormTransMatrix();
}

void Mesh::FormTransMatrix()
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

D3DXMATRIX Mesh::GetTransMatrix()
{
	return trans;
}

void Mesh::SetTransMatrix(D3DXMATRIX matrix)
{	
	trans = matrix;
}



BOOL Mesh::Draw()
{
	HR(mesh->DrawSubset(0));
	return TRUE;
}

VOID Mesh::Release()
{
	mesh->Release();
}