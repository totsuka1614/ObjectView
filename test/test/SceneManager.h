//=============================================================================
//
// �V�[�� �N���X��` [Scene.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once
#include "main.h"
#include "SceneDebug.h"
#include "SceneTitle.h"

#ifndef SCENE
#define SCENE SceneManager::Get()
#endif

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

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	static SceneManager* Get() { return m_pScene; };

	EScene GetID(void) { return m_id; }
	void Change(EScene scene);
	
	CDebug* m_pDebug;
	CTitle* m_pTitle;
private:
};