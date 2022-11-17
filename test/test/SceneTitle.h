//=============================================================================
//
// シーンタイトル クラス定義 [SceneTitle.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once

#include "main.h"
#include "SceneBase.h"

class CTitle : public SceneBase
{
public:
	CTitle() {};
	~CTitle() {};

	void Init();
	void Uninit();
	void Update();
	void Draw();


private:

};
