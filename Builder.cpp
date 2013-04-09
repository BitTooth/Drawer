#include "Builder.h"
#include <iostream>
#include <fstream>

void Builder::Init()
{
	m_ar.Init();
}

void Builder::LoadImage(const char* path, __int8** colorOut, int &colorCount, short** depthOut, int &depthCount)
{
	std::ifstream in(path, std::ios::binary|std::ios::in);
	if (!in.good())
		return;
	in.read((char*)(&colorCount), sizeof(int));
	in.read((char*)(&depthCount), sizeof(int));

	char *color = new char[colorCount];
	short *depth = new short[depthCount];

	*colorOut = color;
	*depthOut = depth;
	for (int i = 0; i < colorCount; ++i)
	{
		in.read((char*)&color[i], sizeof(char));
	}

	for (int i = 0; i < depthCount; ++i)
	{
		in.read((char*)&depth[i], sizeof(short));
	}

	in.close();
}

void Builder::LoadImages(int count)
{
	m_nImagesCount = count;
	m_pColorInfo = new __int8*[m_nImagesCount];
	m_pDepthInfo = new short*[m_nImagesCount];

	char path[7];
	int colorLength, depthLength;
	for (int i = 0; i < m_nImagesCount; ++i)
	{
		sprintf(path, "%d.kin", i);
		LoadImage(path, &m_pColorInfo[i], colorLength, &m_pDepthInfo[i], depthLength);
	}
}

void Builder::BuildModel(Scene *scene)
{
	// foreach Image
	//		get camera position with ARToolkit
	D3DXMATRIX matrix;
	matrix = m_ar.GetTransformMatrix(m_pColorInfo[0]);
	if (matrix == NULL)
	{
		MessageBoxA(NULL, "Marker not found", "Error", MB_OK);
		return;
	}

	float det = D3DXMatrixDeterminant(&matrix);
	D3DXMatrixInverse(&matrix, &det, &matrix);

	D3DXMATRIX trans;
	D3DXMATRIX rot;
	D3DXMATRIX scale;

	D3DXMatrixIdentity(&trans);
	D3DXMatrixIdentity(&rot);
	D3DXMatrixIdentity(&scale);

	trans._41 = -matrix._41;
	trans._42 = -matrix._42;
	trans._43 = -matrix._43;
	trans._44 = matrix._44;


	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
	{
		rot.m[i][j] = matrix.m[i][j];
	}

	D3DXMatrixScaling(&scale, 100.f, 20.f, 50.f);

	D3DXMATRIX World;

	World = scale*rot*trans;

	(*(scene->GetObjects()))[0]->SetTransMatrix(World);

	
	//		count vertexes
	//		join vertexes
	//		move vertexes into world
	//	build model for DirecX
}