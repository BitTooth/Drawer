#ifndef __OBJECT_H
#define __OBJECT_H

#include "main.h"

class Object
{
public:
	virtual D3DXVECTOR3 GetPos() = 0;
	virtual D3DXVECTOR3 GetScale() = 0;
	virtual D3DXVECTOR3 GetRotation() = 0;
	virtual void SetPos(D3DXVECTOR3 vec) = 0;
	virtual void SetScale(D3DXVECTOR3 vec) = 0;
	virtual void SetRotation(D3DXVECTOR3 vec) = 0;
	
	virtual void SetTransMatrix(D3DXMATRIX matrix) = 0;
	virtual D3DXMATRIX GetTransMatrix() = 0;
	
	virtual BOOL Draw() = 0;
	virtual VOID Release() = 0;
};
#endif