//=============================================================================
//
// シーンデバッグ クラス定義 [SceneDebug.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once

#include "main.h"
#include "SceneBase.h"
#include "grid.h"
#include <list>
#include <string>
#include "Mesh.h"

class CDebug : public SceneBase
{
public:
	CDebug() {}
	~CDebug() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	std::list<std::string>& GetNameList() { return m_NameList; }
	std::list<CMesh*>&  GetSaveList() { return m_SaveList; }

private:
	std::list<std::string> m_NameList;
	std::list<CMesh*> m_SaveList;

};