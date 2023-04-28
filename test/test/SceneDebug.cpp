/******************************************************************************
* 
* @file      SceneDebug.cpp
* @brief     デバッグシーン
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 現在このシーンがメイン
* 
******************************************************************************/
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

/******************************************************************************
* 
* @brief      Init
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       初期化
* @attention  
******************************************************************************/
void CDebug::Init()
{
	//デバイス取得
	ID3D11Device* pDevice = BACKBUFFER->GetDevice();

	//画面分割
	m_bSplit = false;	
	m_split.Init();

	//データロード
	DataLoad(m_NameList);

	//RenderTarget------------------------------------------

	//メインキャンバス
	CRenderTarget* render = new CRenderTarget;
	render->Create(DXGI_FORMAT_B8G8R8A8_UNORM);
	Entry<CRenderTarget>("Canvas", render);	

	//シャドウキャンバス
	CRenderTarget* render2 = new CRenderTarget;
	render2->Create(DXGI_FORMAT_R32_FLOAT);			
	Entry<CRenderTarget>("ShadowRender", render2);	

	//------------------------------------------------------

	//DepthStencil------------------------------------------

	//シャドウステンシル
	CDepthStencil* depth2 = new CDepthStencil;
	depth2->Create(DXGI_FORMAT_D24_UNORM_S8_UINT);  
	Entry<CDepthStencil>("ShadowDepth", depth2);

	//------------------------------------------------------

	//個別オブジェクト設定----------------------------------
	//リストにはないオブジェクトを個別に設定

	//グリッド生成
	Create<CGrid>("Grid");
	GetComponent<CGrid>("Grid")->Init();
	//プレイヤー生成
	Create<CPlayer>("Player");
	GetComponent<CPlayer>("Player")->Init();

	//-------------------------------------------------------

	//UI設定-------------------------------------------------

	//プレイアイコン生成
	Create<CPlayIcon>("PlayIcon");
	GetComponent<CPlayIcon>("PlayIcon")->Init();

	//回転アイコン(Left)生成
	Create<CRotIcon>("LRotIcon");
	GetComponent<CRotIcon>("LRotIcon")->Init(XMFLOAT2(-500.0f, -280.0f), 1);	
	GetComponent<CRotIcon>("LRotIcon")->SetPlayer(GetComponent<CPlayer>("Player"));

	//回転アイコン(Right)生成
	Create<CRotIcon>("RrotIcon");
	GetComponent<CRotIcon>("RrotIcon")->Init(XMFLOAT2(-350.0f, -280.0f), 0);
	GetComponent<CRotIcon>("RrotIcon")->SetPlayer(GetComponent<CPlayer>("Player")); 

	//ジャンプアイコン生成
	Create<CJumpIcon>("JumpIcon");
	GetComponent<CJumpIcon>("JumpIcon")->Init(XMFLOAT2(-200.0f, -280.0f));
	GetComponent<CJumpIcon>("JumpIcon")->SetPlayer(GetComponent<CPlayer>("Player"));

	//停止アイコン生成
	Create<CStopIcon>("StopIcon");
	GetComponent<CStopIcon>("StopIcon")->Init(XMFLOAT2(-50.0f, -280.0f));
	GetComponent<CStopIcon>("StopIcon")->SetPlayer(GetComponent<CPlayer>("Player"));
	//-------------------------------------------------------------------------------------------------------

	//フィルター生成--------------------------------------------------------------
	Create<CFilter>("Filter");
	GetComponent<CFilter>("Filter")->Init();
	GetComponent<CFilter>("Filter")->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	//----------------------------------------------------------------------------
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       終了処理
* @attention  
******************************************************************************/
void CDebug::Uninit()
{
	//終了時にセーブを促す-------------------------------------------------------------
	if (MessageBox(NULL, _T("セーブしますか？"), _T(""), MB_YESNO) == IDYES)
	{
		for (auto list : m_NameList)	//オブジェクトの数分繰り返す
		{
			switch (GetComponent<CObjectBase>(list.data())->GetType())	//オブジェクトタイプごとにセーブ
			{
			case BOX:	//ボックス
				GetComponent<CBox>(list.data())->SaveFile(); break;
			case SPHERE://スフィア(未実装)
				break;
			case FBX:	//FBXモデル
				GetComponent<CModel>(list.data())->SaveFile();
				GetComponent<CModel>(list.data())->Uninit(); break;
			}
		}
		
		//オブジェクトリストセーブ
		DataSave(m_NameList);
		m_NameList.clear();	

		//プレイヤーセーブ
		GetComponent<CPlayer>("Player")->SaveFile();

		//カメラ終了処理
		CCamera::Get()->Uninit();
	}
	//-------------------------------------------------------------------------------

	//プレイヤー終了処理
	GetComponent<CPlayer>("Player")->Uninit();
	//プレイアイコン終了処理
	GetComponent<CPlayIcon>("PlayIcon")->Fin();
	//回転アイコン(Left)終了処理
	GetComponent<CRotIcon>("LRotIcon")->Fin();
	//回転アイコン(Right)終了処理
	GetComponent<CRotIcon>("RrotIcon")->Fin();
	//ジャンプアイコン終了処理
	GetComponent<CJumpIcon>("JumpIcon")->Fin();
	//停止アイコン終了処理
	GetComponent<CStopIcon>("StopIcon")->Fin();
	//フィルター終了処理
	GetComponent<CPolygon>("Filter")->Fin();
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       更新処理
* @attention  
******************************************************************************/
void CDebug::Update()
{
	//オブジェクトを生成-------------------------------------------------------------------------
	/*
	ゲーム内でリアルタイムでオブジェクトを生成する機能
	obj.bCreateがtrueになった時、生成を始める
	*/

	//作成オブジェクトの宣言
	static CREATE_OBJECT obj;

	if (!GLOBALDATA->GetStartFlag())	//ゲーム開始されていない場合
		obj = IMGUI->DebugDisplay();	//デバッグを表示(生成したオブジェクトを返す。なければbCreateをfalseで返す)
	
	if (obj.bCreate)		//オブジェクトが生成された場合
	{
		//同じ名前が使われているかの判別
		bool bNameFlag = false;

		switch (obj.type)	//オブジェクトタイプごとに生成
		{
		case BOX:			//ボックス
			bNameFlag = Create<CBox>(obj.cName);		//生成(もし同じ名前が使われていたら失敗(false))
	
			if (!bNameFlag)	//同じ名前が使われていた場合
				break;
			//初期化
			GetComponent<CBox>(obj.cName)->SetName(obj.cName);
			GetComponent<CBox>(obj.cName)->Init(XMFLOAT3(1.0f, 1.0f, 1.0f));

			//リストに登録
			m_NameList.push_back(obj.cName);
			break;
		case SPHERE:		//未実装
			break;
		case FBX:			//FBXモデル
			bNameFlag = Create<CModel>(obj.cName);	//生成(もし同じ名前が使われていたら失敗(false))
			if (!bNameFlag)
				break;
			
			//初期化
			GetComponent<CModel>(obj.cName)->SetName(obj.cName);
			GetComponent<CModel>(obj.cName)->SetFileName(obj.cPath);
			GetComponent<CModel>(obj.cName)->Init();

			//リストに登録
			m_NameList.push_back(obj.cName);
			break;
		}
	
		//ポップアップ表示
		if(!bNameFlag)
			MessageBox(NULL, _T("同じ名前は使えません"), _T("error"), MB_OK);
	
	
	}

	//----------------------------------------------------------------------------------------------

	//更新-----------------------------------------------------------------------------
	for (auto list : m_NameList)	//オブジェクトの数分繰り返す
	{
		switch (GetComponent<CObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<CBox>(list.data())->Update(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<CModel>(list.data())->Update(); break;
		}
	}

	//更新処理
	GetComponent<CPlayer>("Player")->Update();
	GetComponent<CGrid>("Grid")->Update();
	GetComponent<CPlayIcon>("PlayIcon")->Update();
	GetComponent<CRotIcon>("LRotIcon")->Update();
	GetComponent<CRotIcon>("RrotIcon")->Update();
	GetComponent<CJumpIcon>("JumpIcon")->Update();
	GetComponent<CStopIcon>("StopIcon")->Update();
	GetComponent<CFilter>("Filter")->Update();
	//---------------------------------------------------------------------------------------------

	//画面分割機能(Control + 1)
	if (CInput::GetKeyPress(VK_LCONTROL))
		if(CInput::GetKeyTrigger(VK_1))
			m_bSplit = !m_bSplit;

	//途中セーブ機能(Control + S)
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	{
		if (GetAsyncKeyState('S') & 0x8000)
		{
			MessageBox(NULL, _T("セーブしました"), _T(""), MB_OK);

			for (auto list : m_NameList)//オブジェクトの数分繰り返す
			{
				switch (GetComponent<CObjectBase>(list.data())->GetType())
				{
				case BOX:
					GetComponent<CBox>(list.data())->SaveFile(); break;
				case SPHERE:
					break;
				case FBX:
					GetComponent<CModel>(list.data())->SaveFile();
					GetComponent<CModel>(list.data())->Uninit(); break;
				}
			}
			DataSave(m_NameList);

			GetComponent<CPlayer>("Player")->SaveFile();
		}
	}
}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       描画処理
* @attention  
******************************************************************************/
void CDebug::Draw()
{
	//バッファゲット
	CBackBuffer* buffer = BACKBUFFER;
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//RenderingTargetの設定	 - ToDo : もっと簡素にする
	CRenderTarget* pRtv = GetComponent<CRenderTarget>("Canvas");
	ID3D11RenderTargetView* pView = pRtv->GetView();

	//シャドウキャンバスに変更
	CRenderTarget* pShadowRtv = GetComponent<CRenderTarget>("ShadowRender");
	CDepthStencil* pShadowDsv = GetComponent<CDepthStencil>("ShadowDepth");

	//シャドウキャンバスをセット
	pView = pShadowRtv->GetView();
	buffer->GetDeviceContext()->ClearRenderTargetView(pView, color);
	buffer->GetDeviceContext()->ClearDepthStencilView(pShadowDsv->GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	buffer->GetDeviceContext()->OMSetRenderTargets(1, &pView, pShadowDsv->GetView());

	//カメラを太陽の位置に
	CCamera::Get()->SetSun();

	//描画-----------------------------
	buffer->SetUpViewPort();
	//DrawDepthShadow();
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
		//DrawShadow(pShadowRtv->GetResource());
		DrawObj();
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

/******************************************************************************
* 
* @brief      SplitDraw
* @param[in]  ShadowTex
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       分割描画(上から、横から、前から、現在のカメラの４つ)
* @attention  
******************************************************************************/
void CDebug::SplitDraw(ID3D11ShaderResourceView* ShadowTex)
{
	//バッファゲット
	CBackBuffer* buffer = BACKBUFFER;

	//画面クリア
	CCamera::Get()->Clear();

	//左上
	buffer->SetUpViewPort(0.0f, 0.0f, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	m_split.SetMode(UP_VIEW);
	CCamera::Set(&m_split);
	CCamera::Get()->Sky();
	DrawShadow(ShadowTex);
	
	//右上
	buffer->SetUpViewPort(SCREEN_CENTER_X, 0.0f, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	m_split.SetMode(SIDE_VIEW);
	CCamera::Get()->Sky();
	DrawShadow(ShadowTex);

	//左下
	buffer->SetUpViewPort(0.0f, SCREEN_CENTER_Y, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	m_split.SetMode(FRONT_VIEW);
	CCamera::Get()->Sky();
	DrawShadow(ShadowTex);

	//右下
	CCamera::Set();
	buffer->SetUpViewPort(SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	CCamera::Get()->Sky();
	DrawShadow(ShadowTex);

	//ビューポート設定を元に戻す
	buffer->SetUpViewPort();
	//UI描画
	Draw2D();

}

/******************************************************************************
* 
* @brief      DrawObj
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       オブジェクト描画
* @attention  
******************************************************************************/
void CDebug::DrawObj()
{
	//描画
	GetComponent<CGrid>("Grid")->Draw();
	GetComponent<CPlayer>("Player")->Draw();
	
	for (auto list : m_NameList)	//オブジェクトの数分繰り返す
	{
		switch (GetComponent<CObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<CBox>(list.data())->Draw(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<CModel>(list.data())->Draw(); break;
		}

	}
}

/******************************************************************************
* 
* @brief      Draw2D
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       2Dオブジェクト描画
* @attention  Zバッファを無効にし、描画後に有効に戻すこと
******************************************************************************/
void CDebug::Draw2D()
{
	//バッファゲット
	CBackBuffer* buffer = BACKBUFFER;

	//Zバッファ無効
	buffer->SetZBuffer(false);
	
	//描画
	buffer->SetBlendState(BS_ALPHABLEND);
	GetComponent<CPlayIcon>("PlayIcon")->Draw();
	GetComponent<CRotIcon>("LRotIcon")->Draw();
	GetComponent<CRotIcon>("RrotIcon")->Draw();
	GetComponent<CJumpIcon>("JumpIcon")->Draw();
	GetComponent<CStopIcon>("StopIcon")->Draw();
	buffer->SetBlendState();
	
	//Zバッファ有効
	buffer->SetZBuffer(true);
}

/******************************************************************************
* 
* @brief      DrawDepthShadow
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       シャドウマップに書き込み用
* @attention  
******************************************************************************/
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


	for (auto list : m_NameList)//オブジェクトの数分繰り返す
	{
		i = GetComponent<CObjectBase>(list.data())->GetVSType();
		f = GetComponent<CObjectBase>(list.data())->GetPSType();

		GetComponent<CObjectBase>(list.data())->SetVSType(DEPTHWRITEVS);
		GetComponent<CObjectBase>(list.data())->SetPSType(DEPTHWRITEPS);

		switch (GetComponent<CObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<CBox>(list.data())->Draw(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<CModel>(list.data())->Draw(); break;
		}

		GetComponent<CObjectBase>(list.data())->SetVSType(i);
		GetComponent<CObjectBase>(list.data())->SetPSType(f);

	}
}

/******************************************************************************
* 
* @brief      DrawShadow
* @param[in]  ShadowTex
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       シャドウマップを利用した描画
* @attention  
******************************************************************************/
void CDebug::DrawShadow(ID3D11ShaderResourceView* ShadowTex)
{
	//シャドウマップ設定
	BACKBUFFER->SetTexture(ShadowTex, 1);
	
	//ぐちゃぐちゃですが、シェーダをSHADOWVS,DEPTHSHADOWPSにセットして描画してるだけです
	//描画
	VSShaderType i = GetComponent<CPlayer>("Player")->GetVSType();
	PSShaderType f = GetComponent<CPlayer>("Player")->GetPSType();
	GetComponent<CPlayer>("Player")->SetVSType(SHADOWVS);
	GetComponent<CPlayer>("Player")->SetPSType(DEPTHSHADOWPS);
	GetComponent<CGrid>("Grid")->Draw();
	GetComponent<CPlayer>("Player")->Draw();
	GetComponent<CPlayer>("Player")->SetVSType(i);
	GetComponent<CPlayer>("Player")->SetPSType(f);

	for (auto list : m_NameList)	//オブジェクトの数分繰り返す
	{
		i = GetComponent<CObjectBase>(list.data())->GetVSType();
		f = GetComponent<CObjectBase>(list.data())->GetPSType();

		GetComponent<CObjectBase>(list.data())->SetVSType(SHADOWVS);
		GetComponent<CObjectBase>(list.data())->SetPSType(DEPTHSHADOWPS);

		switch (GetComponent<CObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<CBox>(list.data())->Draw(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<CModel>(list.data())->Draw(); break;
		}

		GetComponent<CObjectBase>(list.data())->SetVSType(i);
		GetComponent<CObjectBase>(list.data())->SetPSType(f);

	}
}