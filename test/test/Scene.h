//=============================================================================
//
// �V�[�� �N���X��` [Scene.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once
#include "main.h"

// �萔��`
enum EScene {
	SCENE_NONE,		
	SCENE_TITLE,		
	SCENE_GAME,			

	MAX_SCENE
};

// �N���X��`
class SceneManager {
protected:
	EScene m_id;

private:
	static SceneManager* m_pScene;

public:
	SceneManager();
	~SceneManager();

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static void Change(EScene scene);

};
