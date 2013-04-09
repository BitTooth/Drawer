#include "vertex.h"
//#include "Render.h"

IDirect3DVertexDeclaration9 *VertexPNDecl;
IDirect3DVertexDeclaration9 *ColorVertexDecl;

BOOL DeclareColorVertex(IDirect3DDevice9 *m_device)
{
	HR(m_device->CreateVertexDeclaration(ColorVertexDeclArr, &ColorVertexDecl));
	if (!ColorVertexDecl)
		return FALSE;
	return TRUE;
}