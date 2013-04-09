#include "ARtoolkit.h"
#include <iostream>

void ARToolkit::Init()
{
	ARParam wparam, cparam;

	if (arParamLoad("camera_para.dat", 1, &wparam) < 0)
	{
		std::cout<<"Error loading camera param\n";
	}
	int a = 0;
	a = arParamChangeSize(&wparam, 640, 480, &cparam);
	a = arInitCparam(&cparam);
	a = arParamDisp(&cparam);

	m_patt_name = "patt.hiro";
	m_patt_id = arLoadPatt(m_patt_name);
	m_nTreshold = 100;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			m_resMatrix[i][j] = (i==j)? 1. : 0.;
		}
	}

	argInit( &cparam, 1.0, 0, 0, 0, 0 );
}

bool ARToolkit::DetectMarker(__int8* data, ARMarkerInfo** markerInfo, int* markerNum)
{
	argDrawMode2D();
	argDispImage((ARUint8*)data, 0,0 );
	argSwapBuffers();

	int res = arDetectMarker((ARUint8*)data, m_nTreshold, markerInfo, markerNum);
	if (res < 0)
	{
		return false;
	}

	int i = 1;
	return true;
}

D3DXMATRIX ARToolkit::GetTransformMatrix( __int8* data )
{
	ARMarkerInfo* markerInfo;
	int markerNum;
	double patt_center[2] = { 0.0, 0.0 };
	double patt_width = 80.0;
	double matrix[3][4];
	D3DXMATRIX resMatrix;
	D3DXMatrixIdentity(&resMatrix);

	if (!DetectMarker(data, &markerInfo, &markerNum)) 
		return resMatrix;

	arGetTransMat(markerInfo, patt_center, patt_width, matrix);

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 4; ++j)
			resMatrix.m[i][j] = matrix[i][j];

	D3DXMatrixTranspose(&resMatrix, &resMatrix);

	return resMatrix;
}