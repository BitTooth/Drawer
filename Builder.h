#ifndef __BUILDER_H
#define __BUILDER_H

#include "main.h"

#define V_FOV 43.f
#define H_FOV 57.f

struct TempVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
};

class Builder
{
private:
	int m_nImagesCount;
	ARToolkit m_ar;
	__int8 **m_pColorInfo;
	short **m_pDepthInfo;
	float depthThresh;

	int f;	// focal length
	int cx;	// x principal point
	int cy;	// y principal point

	std::vector<std::vector<TempVertex> > tempVertex;
	bool isDead(int i, int j);
	bool CheckDepth(int i1, int j1, int i2, int j2);
	bool BuildNormal(int i1, int j1, int i2, int j2, int i3, int j3, D3DXVECTOR3 &norm);
public:
	void Init();
	void LoadImage(const char* path, __int8** colorOut, int &colorCount, short** depthOut, int &depthCount);
	void LoadImages(int count);
	void BuildModel(Scene  *scene);
	static float rad(float angle);
	void Release();
};

#endif