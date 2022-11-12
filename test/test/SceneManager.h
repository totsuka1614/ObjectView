//=============================================================================
//
// シーン クラス定義 [Scene.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once
#include "main.h"
#include "SceneDebug.h"

// 定数定義
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
	SceneManager();
	~SceneManager();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	static SceneManager* Get() { return m_pScene; };

	void Change(EScene scene);
	
private:
	CDebug* m_pDebug;
};
