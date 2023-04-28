/******************************************************************************
* 
* @file      SceneManager.h
* @brief     �V�[���}�l�[�W���[�N���X
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//�C���N���[�h��
#include "main.h"
#include "SceneDebug.h"
#include "SceneTitle.h"

//�}�N��
#ifndef SCENE
#define SCENE SceneManager::Get()
#endif

// �V�[����
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
	//�R���X�g���N�^
	SceneManager();
	//�f�X�g���N�^
	~SceneManager();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	static SceneManager* Get() { return m_pScene; };

	EScene GetID(void) { return m_id; }
	void Change(EScene scene);
	
	//�V�[���ϐ�
	CDebug* m_pDebug;
	CTitle* m_pTitle;
private:
};