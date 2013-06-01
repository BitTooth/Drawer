#ifndef __ARTOOLKIT_H
#define __ARTOOLKIT_H


#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <d3d9.h>
#include <d3dx9.h>

class ARToolkit
{
private:
	int m_nTreshold;
	int m_patt_id;
	char *m_patt_name;

	float m_resMatrix[4][4];
public:
	bool DetectMarker(__int8* data, ARMarkerInfo** markerInfo, int* markerNum);
	D3DXMATRIX GetTransformMatrix(__int8* data);
	void Init();
	void Release();
};

#endif