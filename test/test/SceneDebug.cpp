//=============================================================================
//
// シーンデバッグ クラス [SceneDebug.cpp]
// Author : Totsuka Kensuke
//
//=============================================================================
#include "sceneDebug.h"
#include "Player.h"
#include "grid.h"
#include "GUI.h"
#include "UI.h"
#include "input.h"
#include "PlayIcon.h"
#include "CollisionList.h"
#include "GlobalData.h"
#include "RotIcon.h"
#include "JumpIcon.h"
#include "StopIcon.h"
#include "SceneManager.h"
#include "RenderTarget.h"
#include "DepthStencil.h"

void CDebug::Init()
{
	ID3D11Device* pDevice = BACKBUFFER->GetDevice();

	m_bSplit = false;
	m_split.Init();
	//データロード
	DataLoad(m_NameList);

	//RenderTarget------------------------------------------
	RenderTarget* render = new RenderTarget;
	render->Create(DXGI_FORMAT_B8G8R8A8_UNORM);
	Entry<RenderTarget>("Render", render);

	DepthStencil* depth = new DepthStencil;
	depth->Create(DXGI_FORMAT_D24_UNORM_S8_UINT);
	Entry<DepthStencil>("Depth", depth);
	//------------------------------------------------------

	//個別オブジェクト設定----------------------------------
	Create<Grid>("Grid");
	Create<CPlayer>("Player");

	GetComponent<Grid>("Grid")->Init();
	GetComponent<CPlayer>("Player")->Init();
	//-------------------------------------------------------

	//UI設定-------------------------------------------------
	if (Create<CPlayIcon>("PlayIcon"))
		GetComponent<CPlayIcon>("PlayIcon")->Init();
	if (Create<CRotIcon>("LRotIcon"))
	{
		GetComponent<CRotIcon>("LRotIcon")->Init(XMFLOAT2(-500.0f, -280.0f), 1);
		GetComponent<CRotIcon>("LRotIcon")->SetPlayer(GetComponent<CPlayer>("Player"));
	}
	if (Create<CRotIcon>("RrotIcon"))
	{
		GetComponent<CRotIcon>("RrotIcon")->Init(XMFLOAT2(-350.0f, -280.0f), 0);
		GetComponent<CRotIcon>("RrotIcon")->SetPlayer(GetComponent<CPlayer>("Player"));
	}
	if (Create<CJumpIcon>("JumpIcon"))
	{
		GetComponent<CJumpIcon>("JumpIcon")->Init(XMFLOAT2(-200.0f, -280.0f));
		GetComponent<CJumpIcon>("JumpIcon")->SetPlayer(GetComponent<CPlayer>("Player"));
	}
	if (Create<CStopIcon>("StopIcon"))
	{
		GetComponent<CStopIcon>("StopIcon")->Init(XMFLOAT2(-50.0f, -280.0f));
		GetComponent<CStopIcon>("StopIcon")->SetPlayer(GetComponent<CPlayer>("Player"));
	}
	//-------------------------------------------------------

}

void CDebug::Uninit()
{
	//終了時にセーブを促す-------------------------------------------------------------
	if (MessageBox(NULL, _T("セーブしますか？"), _T(""), MB_YESNO) == IDYES)
	{
		for (auto list : m_NameList)
		{
			switch (GetComponent<ObjectBase>(list.data())->GetType())
			{
			case BOX:
				GetComponent<Box>(list.data())->SaveFile(); break;
			case SPHERE:
				break;
			case FBX:
				GetComponent<Model>(list.data())->SaveFile();
				GetComponent<Model>(list.data())->Uninit(); break;
			}
		}
		DataSave(m_NameList);

		m_NameList.clear();

		GetComponent<CPlayer>("Player")->SaveFile();

		CCamera::Get()->Uninit();
	}
	//-------------------------------------------------------------------------------

	GetComponent<CPlayer>("Player")->Uninit();

	GetComponent<CPlayIcon>("PlayIcon")->Fin();
	GetComponent<CRotIcon>("LRotIcon")->Fin();
	GetComponent<CRotIcon>("RrotIcon")->Fin();
	GetComponent<CJumpIcon>("JumpIcon")->Fin();
	GetComponent<CStopIcon>("StopIcon")->Fin();

}

void CDebug::Update()
{
	/*if (GLOBALDATA->GetClearFlag())
	{
		SCENE->Change(SCENE_TITLE);
		return;
	}*/

	//オブジェクトを生成-------------------------------------------------------------------------
	static CREATE_OBJECT obj;

	if (!GLOBALDATA->GetStartFlag())
	{
		obj = GUI::Get()->DebugDisplay();
	}
	if (obj.bCreate)
	{
		bool bflag = false;

		switch (obj.type)
		{
		case BOX:
			bflag = Create<Box>(obj.cName);
			if (!bflag)
				break;
			GetComponent<Box>(obj.cName)->SetName(obj.cName);
			GetComponent<Box>(obj.cName)->Init(XMFLOAT3(1.0f, 1.0f, 1.0f));
			m_NameList.push_back(obj.cName);
			break;
		case SPHERE:
			break;
		case FBX:
			bflag = Create<Model>(obj.cName);
			if (!bflag)
				break;
			GetComponent<Model>(obj.cName)->SetName(obj.cName);
			GetComponent<Model>(obj.cName)->SetFileName(obj.cPath);
			GetComponent<Model>(obj.cName)->Init();
			m_NameList.push_back(obj.cName);
			break;
		}
	
		if(!bflag)
			MessageBox(NULL, _T("同じ名前は使えません"), _T("error"), MB_OK);
	
	
	}

	//----------------------------------------------------------------------------------------------

	for (auto list : m_NameList)
	{
		switch (GetComponent<ObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<Box>(list.data())->Update(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<Model>(list.data())->Update(); break;
		}
	}

	GetComponent<CPlayer>("Player")->Update();
	GetComponent<Grid>("Grid")->Update();
	GetComponent<CPlayIcon>("PlayIcon")->Update();
	GetComponent<CRotIcon>("LRotIcon")->Update();
	GetComponent<CRotIcon>("RrotIcon")->Update();
	GetComponent<CJumpIcon>("JumpIcon")->Update();
	GetComponent<CStopIcon>("StopIcon")->Update();

	if (CInput::GetKeyTrigger(VK_SPACE))
		m_bSplit = !m_bSplit;

	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	{
		if (GetAsyncKeyState('S') & 0x8000)
		{
			MessageBox(NULL, _T("セーブしました"), _T(""), MB_OK);

			for (auto list : m_NameList)
			{
				switch (GetComponent<ObjectBase>(list.data())->GetType())
				{
				case BOX:
					GetComponent<Box>(list.data())->SaveFile(); break;
				case SPHERE:
					break;
				case FBX:
					GetComponent<Model>(list.data())->SaveFile();
					GetComponent<Model>(list.data())->Uninit(); break;
				}
			}
			DataSave(m_NameList);

			GetComponent<CPlayer>("Player")->SaveFile();
		}
	}

	if (GLOBALDATA->GetClearFlag())
	{
		//GLOBALDATA->Clear();
		//SCENE->Change(SCENE_TITLE);
	}
}

void CDebug::Draw()
{
	//バッファゲット
	BackBuffer* buffer = BACKBUFFER;

	//RenderingTargetの設定	 - ToDo : もっと簡素にする
	RenderTarget* pRtv = GetComponent<RenderTarget>("Render");
	ID3D11RenderTargetView* pView = pRtv->GetView();
	DepthStencil* pDsv = GetComponent<DepthStencil>("Depth");
	buffer->GetDeviceContext()->OMSetRenderTargets(1, &pView , pDsv->GetView());

	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	buffer->GetDeviceContext()->ClearRenderTargetView(pView, color);
	buffer->GetDeviceContext()->ClearDepthStencilView(pDsv->GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f,0);
		
	DrawObj();
	

	pView = buffer->GetRenderTargetView();
	buffer->GetDeviceContext()->OMSetRenderTargets(1, &pView , buffer->GetDepthStencilView());

	if (!m_bSplit)
	{
		buffer->SetUpViewPort();
		DrawObj();
		Draw2D();
	}
	else
	{
		SplitDraw();
	}

}

void CDebug::SplitDraw()
{
	//バッファゲット
	BackBuffer* buffer = BACKBUFFER;

	ID3D11RenderTargetView* pView = buffer->GetRenderTargetView();
	buffer->GetDeviceContext()->OMSetRenderTargets(1, &pView, buffer->GetDepthStencilView());

	CCamera::Get()->Clear();

	buffer->SetUpViewPort(0.0f, 0.0f, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	m_split.SetMode(UP_VIEW);
	CCamera::Set(&m_split);
	CCamera::Get()->Sky();
	DrawObj();
	
	buffer->SetUpViewPort(SCREEN_CENTER_X, 0.0f, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	m_split.SetMode(SIDE_VIEW);
	CCamera::Get()->Sky();
	DrawObj();

	buffer->SetUpViewPort(0.0f, SCREEN_CENTER_Y, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	m_split.SetMode(FRONT_VIEW);
	CCamera::Get()->Sky();
	DrawObj();

	CCamera::Set();
	buffer->SetUpViewPort(SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	CCamera::Get()->Sky();
	DrawObj();

	buffer->SetUpViewPort();
	Draw2D();

}

void CDebug::DrawObj()
{
	GetComponent<Grid>("Grid")->Draw();
	GetComponent<CPlayer>("Player")->Draw();

	for (auto list : m_NameList)
	{
		switch (GetComponent<ObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<Box>(list.data())->Draw(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<Model>(list.data())->Draw(); break;
		}
	}
}
void CDebug::Draw2D()
{
	//バッファゲット
	BackBuffer* buffer = BACKBUFFER;

	buffer->SetZBuffer(false);
	buffer->SetBlendState(BS_ALPHABLEND);
	GetComponent<CPlayIcon>("PlayIcon")->Draw();
	GetComponent<CRotIcon>("LRotIcon")->Draw();
	GetComponent<CRotIcon>("RrotIcon")->Draw();
	GetComponent<CJumpIcon>("JumpIcon")->Draw();
	GetComponent<CStopIcon>("StopIcon")->Draw();
	buffer->SetBlendState();
	buffer->SetZBuffer(true);
}