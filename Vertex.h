//////////////////////////////////////////////////////////////////////////
//																		//
//		In this file should be all vertex declarations					//
//																		//
//////////////////////////////////////////////////////////////////////////

#ifndef __VERTEX_H
#define __VERTEX_H
//#include "d3dUtil.h"
#include "main.h"
//#include "Render.h"


static D3DVERTEXELEMENT9 ColorVertexDeclArr[] =
	{
		{0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0},
		D3DDECL_END()
	};

static D3DVERTEXELEMENT9 VertexPNDeclArr[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		D3DDECL_END()
	};

extern IDirect3DVertexDeclaration9 *ColorVertexDecl;
extern IDirect3DVertexDeclaration9 *VertexPNDecl;

struct ColorVertex
{
	D3DXVECTOR3 _pos;
	D3DCOLOR _color;
};

struct VertexPN
{
	VertexPN():pos(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f){}
	VertexPN(float x, float y, float z, float nx, float ny, float nz):
		pos(x, y, z), normal(nx, ny, nz){}
	VertexPN(const D3DXVECTOR3 &v, const D3DXVECTOR3 &n):
		pos(v), normal(n){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
};

BOOL DeclareColorVertex();
BOOL DeclareVertexPN();

#endif