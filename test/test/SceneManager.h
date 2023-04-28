/******************************************************************************
* 
* @file      SceneManager.h
* @brief     シーンマネージャークラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//インクルード部
#include "main.h"
#include "SceneDebug.h"
#include "SceneTitle.h"

//マクロ
#ifndef SCENE
#define SCENE SceneManager::Get()
#endif

// シーン列挙
enum EScene {
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_DEBUG,
	MAX_SCENE
};

// クラス定義
class SceneManager {
protected:
	EScene m_id;
private:
	static SceneManager* m_pScene;
public:
	//コンストラクタ
	SceneManager();
	//デストラクタ
	~SceneManager();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	static SceneManager* Get() { return m_pScene; };

	EScene GetID(void) { return m_id; }
	void Change(EScene scene);
	
	//シーン変数
	CDebug* m_pDebug;
	CTitle* m_pTitle;
private:
};