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

class CDebug : public SceneBase
{
public:
	CDebug() {}
	~CDebug() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	std::list<std::string> m_NameList;

};