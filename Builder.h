#ifndef __BUILDER_H
#define __BUILDER_H

#include "main.h"

// #include "Scene.h"

class Builder
{
private:
	int m_nImagesCount;
	ARToolkit m_ar;
	__int8 **m_pColorInfo;
	short **m_pDepthInfo;

public:
	void Init();
	void LoadImage(const char* path, __int8** colorOut, int &colorCount, short** depthOut, int &depthCount);
	void LoadImages(int count);
	void BuildModel(Scene  *scene);
};

#endif