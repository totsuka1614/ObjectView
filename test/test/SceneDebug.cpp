//=============================================================================
//
// シーンデバッグ クラス [SceneDebug.cpp]
// Author : Totsuka Kensuke
//
//=============================================================================
#include "sceneDebug.h"

void CDebug::Init()
{
	Create<Model>("Player");
	
	Get<Model>("Player")->Init();

}

void CDebug::Uninit()
{
	Get<Model>("Player")->Uninit();
}

void CDebug::Update()
{
	Get<Model>("Player")->Update();
}

void CDebug::Draw()
{
	Get<Model>("Player")->Draw();
}