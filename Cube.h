#ifndef __CUBE_H
#define __CUBE_H

#include "main.h"
// #include "d3dUtil.h"

class Cube
{
private:
	ID3DXMesh *mesh;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 rotation;
	
	D3DXMATRIX trans;
	void FormTransMatrix();
public:
	Cube(IDirect3DDevice9 *m_device);
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetScale();
	D3DXVECTOR3 GetRotation();
	void SetPos(D3DXVECTOR3 vec);
	void SetScale(D3DXVECTOR3 vec);
	void SetRotation(D3DXVECTOR3 vec);

	void SetTransMatrix(D3DXMATRIX matrix);
	D3DXMATRIX GetTransMatrix();

	virtual BOOL Draw();
	virtual VOID Release();
};

#endif