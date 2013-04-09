#include "Render.h"
// #include "application.h"
// #include "ConfigLoader.h"


BOOL Render::Init(Window *wnd, Builder *build, bool windowed)
{
	m_window = wnd;
	m_builder = build;
	m_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	m_windowed = windowed;
	
	m_param.BackBufferWidth				= m_window->GetWidth();
	m_param.BackBufferHeight			= m_window->GetHeight();
	m_param.Windowed					= m_windowed;
	m_param.BackBufferFormat			= D3DFMT_A8R8G8B8;
	m_param.BackBufferCount				= 1;
	m_param.MultiSampleType				= D3DMULTISAMPLE_NONE;
	m_param.MultiSampleQuality			= 0;
	m_param.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	m_param.hDeviceWindow				= m_window->GetWnd();
	m_param.EnableAutoDepthStencil		= true;
	m_param.AutoDepthStencilFormat		= D3DFMT_D24S8;
	m_param.Flags						= 0;
	m_param.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	m_param.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

	m_device = NULL;
	HR(m_d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_window->GetWnd(), 
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_param, &m_device));

	if (!m_device)
		return FALSE;

	if(!AdditionalInitialization())
		return FALSE;

	return TRUE;
}

VOID Render::OnPaint(Scene *scene)
{
	HR(m_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));

	if (SUCCEEDED(m_device->BeginScene()))
	{
		//TODO: Drawing here

		//Setting camera
		D3DXVECTOR3 camPos = scene->camera->m_pos;
		D3DXVECTOR3 camTarget = scene->camera->m_target;
		D3DXVECTOR3 camUp = scene->camera->m_up;

		D3DXMATRIX camMatrix;
		D3DXMatrixLookAtLH(&camMatrix, &camPos, &camTarget, &camUp); 
		HR(m_device->SetTransform(D3DTS_VIEW, &camMatrix));

		//Building projection matrix
		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.25f, 
			(float)m_param.BackBufferWidth/(float)m_param.BackBufferHeight,
			1.0f, 2000.0f);

		HR(m_device->SetTransform(D3DTS_PROJECTION, &proj));

		//Loading shader
		ghWVP = mFX->GetParameterByName(NULL, "gWorldViewProjection");
		ghWorldInverseTransform = mFX->GetParameterByName(NULL, "gWorldInverseTranspose");
		ghDiffuseLight = mFX->GetParameterByName(NULL, "gDiffuseLight");
		ghDiffuseMtl = mFX->GetParameterByName(NULL, "gDiffuseMtl");
		ghLightVecW = mFX->GetParameterByName(NULL, "gLightVecW");
		tech = mFX->GetTechniqueByName("TransformTech");
		mFX->SetTechnique(tech);

		//Drawing objects from Scene
		std::vector<Cube*> *objects;
		std::vector<Cube*>::iterator it;
		objects = scene->GetObjects();
		for (it = objects->begin(); it != objects->end(); ++it)
		{
			D3DXMATRIX world;
			
// 			D3DXMATRIX trans;
// 			D3DXMATRIX scale;
// 			D3DXMATRIX rot;
// 
// 			D3DXMatrixIdentity(&trans);
// 			D3DXMatrixIdentity(&scale);
// 			D3DXMatrixIdentity(&rot);
// 
// 			D3DXMatrixTranslation(&trans, (*it)->GetPos().x, (*it)->GetPos().y, (*it)->GetPos().z);
// 			D3DXMatrixScaling(&scale, (*it)->GetScale().x, (*it)->GetScale().y, (*it)->GetScale().z);
// 			D3DXMatrixRotationYawPitchRoll(&rot, (*it)->GetRotation().x, (*it)->GetRotation().y, (*it)->GetRotation().z);
// 			// world = trans*rot*scale;
// 			world = scale*rot*trans;
			world = (*it)->GetTransMatrix();
			HR(m_device->SetTransform(D3DTS_WORLD, &world));

			D3DXMATRIX WorldInverseTransform;
			D3DXMatrixInverse(&WorldInverseTransform, 0, &world);
			D3DXMatrixTranspose(&WorldInverseTransform, &WorldInverseTransform);
			
			D3DXCOLOR DiffuseMtl(0.4f, 0.4f, 0.4f, 1.0f);
			D3DXCOLOR DiffuseLight(0.2f, 0.2f, 0.2f, 1.0f);
			D3DXVECTOR3 LightVec(1.0f, 5.0f, 10.0f);
			
			HR(mFX->SetMatrix(ghWVP, &(world*camMatrix*proj)));
			HR(mFX->SetMatrix(ghWorldInverseTransform, &WorldInverseTransform));
			HR(mFX->SetValue(ghDiffuseLight, &DiffuseLight, sizeof(D3DXCOLOR)));
			HR(mFX->SetValue(ghDiffuseMtl, &DiffuseMtl, sizeof(D3DXCOLOR)));
			HR(mFX->SetValue(ghLightVecW, &LightVec, sizeof(D3DXVECTOR3)));
			UINT nPasses;
			HR(mFX->Begin(&nPasses, NULL));

			for (int i = 0; i < nPasses; ++i)
			{
				HR(mFX->BeginPass(i));

				(*it)->Draw();

				HR(mFX->EndPass());
			}

		}///for objects///

		HR(m_device->EndScene());
	}

	HR(m_device->Present(NULL, NULL, NULL, NULL));
}

VOID Render::OnDestroy()
{
	ReleaseCOM(m_device);
	ReleaseCOM(m_d3d9);
	ReleaseCOM(mFX);
}

BOOL Render::AdditionalInitialization()
{
	HR(m_device->CreateVertexDeclaration(ColorVertexDeclArr, &ColorVertexDecl));
	HR(m_device->CreateVertexDeclaration(VertexPNDeclArr, &VertexPNDecl));

	//Setting render state
	HR(m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW));
	HR(m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
	HR(m_device->SetRenderState(D3DRS_LIGHTING, FALSE));

	//Loading shader
	ID3DXBuffer* errors = 0;

	HR(D3DXCreateEffectFromFile (m_device, "Diffuse.fx", 
		NULL, NULL, D3DXSHADER_DEBUG, NULL, &mFX, &errors));
	if (errors)
	{
		MessageBox(NULL, (char*)errors->GetBufferPointer(), "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(1);
	}

	ghWVP = 0;
	ghWorldInverseTransform = 0;
	ghDiffuseMtl = 0;
	ghDiffuseLight = 0;
	ghLightVecW = 0;
	tech = 0;

	return TRUE;
}

DWORD Render::IsDeviceLost() 
{
	HRESULT hr = m_device->TestCooperativeLevel();
	switch (hr)
	{
	case (D3DERR_DEVICELOST):			return DEVICE_LOST;		 break;
	case (D3DERR_DRIVERINTERNALERROR):  return DEVICE_ERROR;	 break;
	case (D3DERR_DEVICENOTRESET):		return DEVICE_NOT_RESET; break;
	default:							return DEVICE_NORMAL;	 break;
	}
}

BOOL Render::OnReset()
{
	if (FAILED(m_device->Reset(&m_param)))
		return FALSE;
	return TRUE;
}