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
void CDebug::Init()
{

	SceneBase::LoadData();

	Create<Grid>("Grid");
	Create<CPlayer>("Player");
	
	GetComponent<Grid>("Grid")->Init();
	GetComponent<CPlayer>("Player")->Init();

}

void CDebug::Uninit()
{
	
	GetComponent<CPlayer>("Player")->Uninit();
}

void CDebug::Update()
{
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
			MessageBox(NULL, _T("同じ名前は使えないンゴねぇwwwww"), _T("error"), MB_OK);
		}
	}

	for (auto list : m_NameList)
	{
		GetComponent<Box>(list.data())->Update();
	}

	GetComponent<CPlayer>("Player")->Update();
}

void CDebug::Draw()
{
	GetComponent<Grid>("Grid")->Draw();
	GetComponent<CPlayer>("Player")->Draw();

	for (auto list : m_NameList)
	{
		GetComponent<Box>(list.data())->Draw();
	}
}