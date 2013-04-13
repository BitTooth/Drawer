#ifndef __CUBE_H
#define __CUBE_H

#include "main.h"

class Cube : public Object
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
	virtual D3DXVECTOR3 GetPos();
	virtual D3DXVECTOR3 GetScale();
	virtual D3DXVECTOR3 GetRotation();
	virtual void SetPos(D3DXVECTOR3 vec);
	virtual void SetScale(D3DXVECTOR3 vec);
	virtual void SetRotation(D3DXVECTOR3 vec);
	
	virtual void SetTransMatrix(D3DXMATRIX matrix);
	virtual D3DXMATRIX GetTransMatrix();

	virtual BOOL Draw();
	virtual VOID Release();
};

#endif