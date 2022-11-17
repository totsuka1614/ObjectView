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

static UI* g_PlayIcon;

void CDebug::Init()
{
	ID3D11Device* pDevice = BackBuffer::GetBuffer()->GetDevice();


	//データロード
	DataLoad(m_NameList);

	//個別オブジェクト設定----------------------------------
	Create<Grid>("Grid");
	Create<CPlayer>("Player");
	
	GetComponent<Grid>("Grid")->Init();
	GetComponent<CPlayer>("Player")->Init();
	//-------------------------------------------------------

	//UI設定-------------------------------------------------
	if (Create<UI>("PlayIcon"))
		g_PlayIcon = GetComponent<UI>("PlayIcon");
		
	g_PlayIcon->Init();
	g_PlayIcon->SetTexture("data/Texture/PlayIcon.png");
	g_PlayIcon->SetPos(550.0f, -300.0f);
	g_PlayIcon->SetSize(256.0f / 2.5f, 256.0f / 2.5f);
}

void CDebug::Uninit()
{
	//終了時にセーブを促す-------------------------------------------------------------
	if (MessageBox(NULL, _T("セーブしますか？"), _T("トツカじゃ"), MB_YESNO) == IDYES)
	{
		for (auto list : m_NameList)
		{
			GetComponent<CMesh>(list.data())->SaveFile();
		}
		DataSave(m_NameList);

		GetComponent<CPlayer>("Player")->SaveFile();
	}
	//-------------------------------------------------------------------------------

	GetComponent<CPlayer>("Player")->Uninit();

	g_PlayIcon->Fin();

}

void CDebug::Update()
{
	//オブジェクトを生成-------------------------------------------------------------------------
	static CREATE_OBJECT obj;

	obj = GUI::Get()->DebugDisplay();

	if (obj.bCreate)
	{
		if (Create<Box>(obj.cName))
		{
			m_NameList.push_back(obj.cName);

			switch (obj.type)
			{
			case BOX:
				GetComponent<Box>(obj.cName)->SetName(obj.cName);
				GetComponent<Box>(obj.cName)->Init(XMFLOAT3(1.0f,1.0f,1.0f));
				break;
			case SPHERE:
				break;
			}
		}
		else
		{
			MessageBox(NULL, _T("同じ名前は使えないンゴねぇwwwww"), _T(""), MB_OK);
		}
	}
	//----------------------------------------------------------------------------------------------

	for (auto list : m_NameList)
	{
		GetComponent<Box>(list.data())->Update();
	}

	GetComponent<CPlayer>("Player")->Update();

	g_PlayIcon->Update();

	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	{
		if (GetAsyncKeyState('S') & 0x8000)
		{
			MessageBox(NULL, _T("セーブしますた"), _T("トツカじゃ"), MB_OK);

			for (auto list : m_NameList)
			{
				GetComponent<Box>(list.data())->SaveFile();
			}
			DataSave(m_NameList);

			GetComponent<CPlayer>("Player")->SaveFile();
		}
	}
}

void CDebug::Draw()
{
	BackBuffer* buffer  = BackBuffer::GetBuffer();


	GetComponent<Grid>("Grid")->Draw();
	GetComponent<CPlayer>("Player")->Draw();

	for (auto list : m_NameList)
	{
		GetComponent<Box>(list.data())->Draw();
	}

	buffer->SetZBuffer(false);
	buffer->SetBlendState(BS_ALPHABLEND);
	g_PlayIcon->Draw();
	buffer->SetBlendState();
	buffer->SetZBuffer(true);
}