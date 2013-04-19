#include "Builder.h"
#include <iostream>
#include <fstream>

void Builder::Init()
{
	m_ar.Init();
	f = 600;
	cx = 320;
	cy = 240;
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

bool Builder::isDead(int i, int j)
{
	if (tempVertex[i][j].pos.x == 0.f
		&&	tempVertex[i][j].pos.y == 0.f
		&&	tempVertex[i][j].pos.z == 0.f)
		return true;
	return false;
}

bool Builder::BuildNormal(int i1, int j1, int i2, int j2, int i3, int j3, D3DXVECTOR3 &norm)
{
	if (isDead(i1, j1) || isDead(i2, j2))
		return false;

	D3DXVECTOR3 v1(tempVertex[i1][j1].pos.x - tempVertex[i2][j2].pos.x,
					tempVertex[i1][j1].pos.y - tempVertex[i2][j2].pos.y,
					tempVertex[i1][j1].pos.z - tempVertex[i2][j2].pos.z);
	D3DXVECTOR3 v2(tempVertex[i1][j1].pos.x - tempVertex[i3][j3].pos.x,
					tempVertex[i1][j1].pos.y - tempVertex[i3][j3].pos.y,
					tempVertex[i1][j1].pos.z - tempVertex[i3][j3].pos.z);
	D3DXVec3Cross(&norm, &v1, &v2);
	return true;
}

float Builder::rad(float angle)
{
	return angle * 180.f / 3.14f;
}

void Builder::BuildModel(Scene *scene)
{
	// for each Image
	for (int img = 0; img < m_nImagesCount; ++img)
	{
		//////////////////////////////////////////////////////////////////////
		//						BUILD CAMERA MATRIX							//
		//////////////////////////////////////////////////////////////////////
		D3DXMATRIX matrix;
		matrix = m_ar.GetTransformMatrix(m_pColorInfo[img]);
		if (matrix == NULL)
		{
			MessageBoxA(NULL, "Marker not found", "Error", MB_OK);
			return;
		}
		
		float det = D3DXMatrixDeterminant(&matrix);
		D3DXMatrixInverse(&matrix, &det, &matrix);
		
		matrix._42 *= -1;
		D3DXMATRIX trans;
		D3DXMATRIX rot;
		D3DXMATRIX addRot;
		D3DXMATRIX scale;

		D3DXMatrixIdentity(&trans);
		D3DXMatrixIdentity(&rot);
		D3DXMatrixIdentity(&addRot);
		D3DXMatrixIdentity(&scale);

		trans._41 = matrix._41;
		trans._42 = matrix._42;
		trans._43 = matrix._43;
		trans._44 = matrix._44;


		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
		{
			rot.m[i][j] = matrix.m[i][j];
		}

		// D3DXMatrixRotationYawPitchRoll(&addRot, rad(-180), rad(95), rad(90));

		rot = rot * addRot;


		D3DXMatrixScaling(&scale, 100.f, 20.f, 50.f);

		D3DXMATRIX World;
		D3DXMATRIX ObjectWorld;

		World = scale*rot*trans;
		ObjectWorld = rot*trans;

		Object *kinect = new Cube(scene->GetDevice());
		kinect->SetTransMatrix(World);
		(*(scene->GetObjects())).push_back(kinect);

		// det = D3DXMatrixDeterminant(&matrix);
		// D3DXMatrixInverse(&matrix, &det, &matrix);
	
		//////////////////////////////////////////////////////////////////////
		//					FIND VERTEXES									//
		//////////////////////////////////////////////////////////////////////
		tempVertex.clear();
		TempVertex nullVertex;
		nullVertex.pos.x = 0.f;
		nullVertex.pos.y = 0.f;
		nullVertex.pos.z = 0.f;

		float zmax = 0.f;
		float xmax = 0.f;
		float ymax = 0.f;

		for (int i = 0; i < 480; ++i) {
			std::vector<TempVertex> x;
			tempVertex.push_back(x);
			for (int j = 0; j < 640; ++j) {
				// float* pDepth = reinterpret_cast<float*>(&m_pDepthInfo[i][k*640 + j]);
				// float depth = *pDepth;
				///////////////////////////////////////////////////////////////////////////////
				// z = depth[i][j]
				// x = (i - 480 / 2) * (z + minDistance) * scaleFactor
				// y = (640 / 2 - j) * (z + minDistance) * scaleFactor
				// 
				///////////////////////////////////////////////////////////////////////////////
				// fxir -  focal length
				// cixr - cenrer of image
				// x, y - position of pixel
				// dm - depth
				//
				// xir = fiixr /((x-cxir)*dm);
				// yir = fixr/((y-cyir)*dm);
				// zir = dm;
				// ////////////////////////////////////////////////////////////////////////////
				// // float minDistance = -10;
				// float scaleFactor = 0.21;
				// depth = 100/(-0.00307*depth + 3.33);
				// 
				// float x = (k - 480 / 2) * (depth + minDistance) * scaleFactor;    
				// float y = (640 / 2 - j) * (depth + minDistance) * scaleFactor;
				// float z = depth;

				// D3DXVECTOR3 vec(x, y, z);
				// D3DXVECTOR4 res(x, y, z, 0.f);
				// D3DXVec3Transform(&res, &vec, &matrix);
				// ////////////////////////////////////////////////////////////////////////////
				short depth = (short)(m_pDepthInfo[img][i*640 + j]);
				if (depth == 0 || j == cx || i == cy) {
					tempVertex[i].push_back(nullVertex);
					continue;
				}

				depth = ~(depth >> 3);

				if (depth == 0 || j == cx || i == cy) {
					tempVertex[i].push_back(nullVertex);
					continue;
				}

				TempVertex t;
				// t.pos.x = (float)f / ((float)(j - cx)*(float)depth);
				// t.pos.y = (float)f / ((float)(i - cy)*(float)depth);
				// t.pos.z = (float)depth;

				t.pos.x = ((float)(j - cx)*(float)depth) / (float)f;
				t.pos.y = ((float)(i - cy)*(float)depth) / (float)f;
				t.pos.z = (float)depth;

				if (abs(t.pos.x) > abs(xmax)) xmax = t.pos.x;
				if (abs(t.pos.y) > abs(ymax)) ymax = t.pos.y;
				if (abs(t.pos.z) > abs(zmax)) zmax = t.pos.z;

				tempVertex[i].push_back(t);
			}
		}

		//////////////////////////////////////////////////////////////////////
		//					JOIN VERTEXES AND FIND NORMALS					//
		//////////////////////////////////////////////////////////////////////

		std::vector<int> tempIndex;

		for (int i = 1; i < 480 - 1; ++i)
			for (int j = 1; j < 640 - 1; ++j)
			{
				if(isDead(i, j)) continue;

				// Triangulate
				if(!isDead(i + 1, j) && !isDead(i+1, j+1))
				{	
					tempIndex.push_back((i+1)*640+(j+1));	//	|\ 
															//	| \ 
					tempIndex.push_back((i+1)*640+j);		//	|  \ 
					tempIndex.push_back(i*640+j);			//	|___\ 
				}
				if(!isDead(i, j+1) && !isDead(i+1, j+1))
				{												//	\----|
					tempIndex.push_back(i*640+(j+1));			//	 \   |
					tempIndex.push_back((i+1)*640+(j+1));		//	  \  |
					tempIndex.push_back(i*640+j);				//	   \ |
				}

				// Find normal
				float sumx = 0.f;
				float sumy = 0.f;
				float sumz = 0.f;
				int count = 0;
				D3DXVECTOR3 tmp;
				if (BuildNormal(i, j, i-1, j, i-1, j-1, tmp)) {
					count++;
					sumx += tmp.x; sumy += tmp.y; sumz += tmp.z;
				}
				if (BuildNormal(i, j, i-1, j-1, i, j-1, tmp)) {
					count++;
					sumx += tmp.x; sumy += tmp.y; sumz += tmp.z;
				}
				if (BuildNormal(i, j, i, j-1, i+1, j, tmp)) {
					count++;
					sumx += tmp.x; sumy += tmp.y; sumz += tmp.z;
				}
				if (BuildNormal(i, j, i+1, j, i+1, j+1, tmp)) {
					count++;
					sumx += tmp.x; sumy += tmp.y; sumz += tmp.z;
				}
				if (BuildNormal(i, j, i+1, j+1, i, j+1, tmp)) {
					count++;
					sumx += tmp.x; sumy += tmp.y; sumz += tmp.z;
				}
				if (BuildNormal(i, j, i, j+1, i+1, j, tmp)) {
					count++;
					sumx += tmp.x; sumy += tmp.y; sumz += tmp.z;
				}

				if (count == 0 || isDead(i, j))
				{
					//std::cout<<"!!! Something went terrible wrong !!!"<<std::endl;
					tempVertex[i][j].normal.x = 0.f;
					tempVertex[i][j].normal.y = 1.f;
					tempVertex[i][j].normal.z = 0.f;
				} else {
					tempVertex[i][j].normal.x = sumx / (float)count;
					tempVertex[i][j].normal.y = sumy / (float)count;
					tempVertex[i][j].normal.z = sumz / (float)count;
				}

			}

			//////////////////////////////////////////////////////////////////////
			//			FILL VERTEX ARRAY, INDEX ARRAY AND CREATE MESH			//
			//////////////////////////////////////////////////////////////////////
			float *vertex = new float[640*480*6];
			DWORD *index = new DWORD[tempIndex.size()];

			for (int i = 0; i < 480; ++i)
			{
				for (int j = 0; j < 640; ++j)
				{
					vertex[i*640*6 + j*6 + 0] = tempVertex[i][j].pos.x;
					vertex[i*640*6 + j*6 + 1] = tempVertex[i][j].pos.y;
					vertex[i*640*6 + j*6 + 2] = tempVertex[i][j].pos.z;
					vertex[i*640*6 + j*6 + 3] = tempVertex[i][j].normal.x;
					vertex[i*640*6 + j*6 + 4] = tempVertex[i][j].normal.y;
					vertex[i*640*6 + j*6 + 5] = tempVertex[i][j].normal.z;
				}
			}

			for (int i = 0; i < tempIndex.size(); ++i)
				index[i] = tempIndex[i];

			Object *mesh = new Mesh(scene->GetDevice(), vertex, index, 640*480, tempIndex.size());
			// mesh->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
			// mesh->SetRotation(D3DXVECTOR3(0.f, 0.f, 0.f));
			// mesh->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
			mesh->SetTransMatrix(ObjectWorld);
			scene->GetObjects()->push_back(mesh);
	}
}