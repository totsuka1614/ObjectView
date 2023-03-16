/******************************************************************************
* 
* @file      SceneManager.cpp
* @brief     �V�[���Ǘ��N���X
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "SceneManager.h"
#include "Camera.h"
#include "GUI.h"
#include "GlobalData.h"

SceneManager g_SceneManager;	//�C���X�^���X

// �ÓI�����o
SceneManager* SceneManager::m_pScene = &g_SceneManager;	//����

/******************************************************************************
* 
* @brief      SceneManager
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �R���X�g���N�^
* @attention  
******************************************************************************/
SceneManager::SceneManager()
{
	//�n�܂�̃V�[�������߂�
	m_id = SCENE_DEBUG;
}

/******************************************************************************
* 
* @brief      ~SceneManager
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �f�X�g���N�^
* @attention  
******************************************************************************/
SceneManager::~SceneManager()
{
	//SAFE_DELETE(m_pScene);
}

/******************************************************************************
* 
* @brief      Init
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ������
* @attention  
******************************************************************************/
void SceneManager::Init()
{
	//������
	if (m_pScene)
	{
		switch (m_pScene->m_id)	//�ǂ̃V�[�����H
		{
		case SCENE_NONE:
			break;
		case SCENE_TITLE:
			m_pTitle = new CTitle;
			m_pTitle->Init();
			break;
		case SCENE_GAME:
			break;
		case SCENE_DEBUG: 
			m_pDebug = new CDebug;
			m_pDebug->Init();
			break;
		}
	}
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �I������
* @attention  
******************************************************************************/
void SceneManager::Uninit()
{
	//�I������
	if (m_pScene)
	{
		switch (m_pScene->m_id) //�ǂ̃V�[�����H
		{
		case SCENE_NONE:
			break;
		case SCENE_TITLE:
			m_pTitle->Uninit();
			delete m_pTitle;
			break;
		case SCENE_GAME:
			break;
		case SCENE_DEBUG:
			m_pDebug->Uninit();
			delete m_pDebug;
			break;
		}
	}
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �X�V����
* @attention  
******************************************************************************/
void SceneManager::Update()
{
	//�X�V
	if (m_pScene)
	{
		switch (m_pScene->m_id)	//�ǂ̃V�[�����H
		{
		case SCENE_NONE:
			break;
		case SCENE_TITLE:
			m_pTitle->Update();
			break;
		case SCENE_GAME:
			break;
		case SCENE_DEBUG:
			m_pDebug->Update();
			break;
		}
	}
}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �`�揈��
* @attention  
******************************************************************************/
void SceneManager::Draw()
{
	//�`��
	if (m_pScene)
	{
		switch (m_pScene->m_id) //�ǂ̃V�[�����H
		{
		case SCENE_NONE:
			break;
		case SCENE_TITLE:
			m_pTitle->Draw();
			break;
		case SCENE_GAME:
			break;
		case SCENE_DEBUG:
			m_pDebug->Draw();
			break;
		}
	}
}


/******************************************************************************
* 
* @brief      Change
* @param[in]  scene
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �V�[���ύX
* @attention  EScene���Q��
******************************************************************************/
void SceneManager::Change(EScene scene)
{
	//���݂̃V�[�����I��
	if (m_pScene) {
		m_pScene->Uninit();
		GUI::Get()->Uninit();
		GLOBALDATA->Uninit();
	}

	//���̃V�[�����J�n
	if (scene)
	{
		m_id = scene;
		m_pScene->Init();
	}
	
}