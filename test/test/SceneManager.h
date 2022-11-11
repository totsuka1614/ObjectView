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
	SCENE_DEBUG,
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

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static SceneManager* Get() { return m_pScene; };

	void Change(EScene scene);

};
