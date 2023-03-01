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
#include "Filter.h"
#include "light.h"


void CDebug::Init()
{
	ID3D11Device* pDevice = BACKBUFFER->GetDevice();

	m_bSplit = false;
	m_split.Init();
	//データロード
	DataLoad(m_NameList);

	//RenderTarget------------------------------------------
	RenderTarget* render = new RenderTarget;
	render->Create(DXGI_FORMAT_B8G8R8A8_UNORM/*DXGI_FORMAT_R32_FLOAT*/);
	Entry<RenderTarget>("Canvas", render);

	RenderTarget* render2 = new RenderTarget;
	render2->Create(DXGI_FORMAT_R32_FLOAT);
	Entry<RenderTarget>("ShadowRender", render2);

	DepthStencil* depth2 = new DepthStencil;
	depth2->Create(DXGI_FORMAT_D24_UNORM_S8_UINT);
	Entry<DepthStencil>("ShadowDepth", depth2);
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

	if (Create<CFilter>("Filter"))
	{
		GetComponent<CFilter>("Filter")->Init();
		GetComponent<CFilter>("Filter")->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	}
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

	GetComponent<CPolygon>("Filter")->Fin();
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
	GetComponent<CFilter>("Filter")->Update();

	if (CInput::GetKeyPress(VK_LCONTROL))
		if(CInput::GetKeyTrigger(VK_1))
	//		m_bSplit = !m_bSplit;

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
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//RenderingTargetの設定	 - ToDo : もっと簡素にする
	RenderTarget* pRtv = GetComponent<RenderTarget>("Canvas");
	ID3D11RenderTargetView* pView = pRtv->GetView();

	//シャドウキャンバスに変更
	RenderTarget* pShadowRtv = GetComponent<RenderTarget>("ShadowRender");
	DepthStencil* pShadowDsv = GetComponent<DepthStencil>("ShadowDepth");

	//シャドウキャンバスをセット
	pView = pShadowRtv->GetView();
	buffer->GetDeviceContext()->ClearRenderTargetView(pView, color);
	buffer->GetDeviceContext()->ClearDepthStencilView(pShadowDsv->GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	buffer->GetDeviceContext()->OMSetRenderTargets(1, &pView, pShadowDsv->GetView());

	//カメラを太陽の位置に
	CCamera::Get()->SetSun();

	//描画-----------------------------
	buffer->SetUpViewPort();
	DrawDepthShadow();
	//---------------------------------

	//カメラを元の位置に
	CCamera::Set();

	//元のキャンバスに変更＆セット
	pView = pRtv->GetView();
	buffer->GetDeviceContext()->ClearRenderTargetView(pView, color);
	buffer->GetDeviceContext()->ClearDepthStencilView(buffer->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	buffer->GetDeviceContext()->OMSetRenderTargets(1, &pView, buffer->GetDepthStencilView());

	//スカイボックスの描画
	CCamera::Get()->Sky();

	//描画-------------------------------------------------------------------
	if (!m_bSplit)	//分割画面ではない
	{
		buffer->SetUpViewPort();
		DrawShadow(pShadowRtv->GetResource());
		Draw2D();
	}				
	else            //分割画面である
	{
		SplitDraw(pShadowRtv->GetResource());
	}
	//-------------------------------------------------------------------------

	//真の描画----------------------------------------------------------------------------------
	pView = buffer->GetRenderTargetView();
	buffer->GetDeviceContext()->ClearRenderTargetView(pView, color);
	buffer->GetDeviceContext()->OMSetRenderTargets(1, &pView , buffer->GetDepthStencilView());

	GetComponent<CFilter>("Filter")->SetTexture(pRtv->GetResource());
	GetComponent<CFilter>("Filter")->Draw();
	//-------------------------------------------------------------------------------------------*/
}

void CDebug::SplitDraw(ID3D11ShaderResourceView* tex)
{
	//バッファゲット
	BackBuffer* buffer = BACKBUFFER;

	CCamera::Get()->Clear();

	buffer->SetUpViewPort(0.0f, 0.0f, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	m_split.SetMode(UP_VIEW);
	CCamera::Set(&m_split);
	CCamera::Get()->Sky();
	DrawShadow(tex);
	
	buffer->SetUpViewPort(SCREEN_CENTER_X, 0.0f, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	m_split.SetMode(SIDE_VIEW);
	CCamera::Get()->Sky();
	DrawShadow(tex);

	buffer->SetUpViewPort(0.0f, SCREEN_CENTER_Y, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	m_split.SetMode(FRONT_VIEW);
	CCamera::Get()->Sky();
	DrawShadow(tex);

	CCamera::Set();
	buffer->SetUpViewPort(SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	CCamera::Get()->Sky();
	DrawShadow(tex);

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

void CDebug::DrawDepthShadow()
{
	//ぐちゃぐちゃですが、シェーダをDEPTH_WRITEにセットして描画してるだけです

	VSShaderType i = GetComponent<CPlayer>("Player")->GetVSType();
	PSShaderType f = GetComponent<CPlayer>("Player")->GetPSType();
	GetComponent<CPlayer>("Player")->SetVSType(DEPTHWRITEVS);
	GetComponent<CPlayer>("Player")->SetPSType(DEPTHWRITEPS);
	GetComponent<CPlayer>("Player")->Draw();
	GetComponent<CPlayer>("Player")->SetVSType(i);
	GetComponent<CPlayer>("Player")->SetPSType(f);


	for (auto list : m_NameList)
	{
		i = GetComponent<ObjectBase>(list.data())->GetVSType();
		f = GetComponent<ObjectBase>(list.data())->GetPSType();

		GetComponent<ObjectBase>(list.data())->SetVSType(DEPTHWRITEVS);
		GetComponent<ObjectBase>(list.data())->SetPSType(DEPTHWRITEPS);

		//地面だけシェーダSHADOWを適応-------------------------------------------------
		if (!strcmp(list.data(), "Land"))
		{
			continue;
		}
		//-----------------------------------------------------------------------------

		switch (GetComponent<ObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<Box>(list.data())->Draw(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<Model>(list.data())->Draw(); break;
		}

		GetComponent<ObjectBase>(list.data())->SetVSType(i);
		GetComponent<ObjectBase>(list.data())->SetPSType(f);

	}
}

void CDebug::DrawShadow(ID3D11ShaderResourceView* tex)
{
	BACKBUFFER->SetTexture(tex, 1);

	CLight::Get()->GetObj()->Draw();

	VSShaderType i = GetComponent<CPlayer>("Player")->GetVSType();
	PSShaderType f = GetComponent<CPlayer>("Player")->GetPSType();
	GetComponent<CPlayer>("Player")->SetVSType(SHADOWVS);
	GetComponent<CPlayer>("Player")->SetPSType(DEPTHSHADOWPS);
	GetComponent<Grid>("Grid")->Draw();
	GetComponent<CPlayer>("Player")->Draw();
	GetComponent<CPlayer>("Player")->SetVSType(i);
	GetComponent<CPlayer>("Player")->SetPSType(f);

	for (auto list : m_NameList)
	{
		i = GetComponent<ObjectBase>(list.data())->GetVSType();
		f = GetComponent<ObjectBase>(list.data())->GetPSType();

		GetComponent<ObjectBase>(list.data())->SetVSType(SHADOWVS);
		GetComponent<ObjectBase>(list.data())->SetPSType(DEPTHSHADOWPS);


		//地面だけシェーダSHADOWを適応-------------------------------------------------
		//if (!strcmp(list.data(),"Land"))
		//{
		//	i = GetComponent<ObjectBase>(list.data())->GetVSType();
		//	f = GetComponent<ObjectBase>(list.data())->GetPSType();
		//	BACKBUFFER->SetTexture(tex, 1);
		//	GetComponent<ObjectBase>(list.data())->SetVSType(SHADOWVS);
		//	GetComponent<ObjectBase>(list.data())->SetPSType(DEPTHSHADOWPS);
		//}
		//-----------------------------------------------------------------------------

		switch (GetComponent<ObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<Box>(list.data())->Draw(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<Model>(list.data())->Draw(); break;
		}

		GetComponent<ObjectBase>(list.data())->SetVSType(i);
		GetComponent<ObjectBase>(list.data())->SetPSType(f);

	}
}