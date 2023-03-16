/******************************************************************************
* 
* @file      SceneManager.cpp
* @brief     シーン管理クラス
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

SceneManager g_SceneManager;	//インスタンス

// 静的メンバ
SceneManager* SceneManager::m_pScene = &g_SceneManager;	//現在

/******************************************************************************
* 
* @brief      SceneManager
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       コンストラクタ
* @attention  
******************************************************************************/
SceneManager::SceneManager()
{
	//始まりのシーンを決める
	m_id = SCENE_DEBUG;
}

/******************************************************************************
* 
* @brief      ~SceneManager
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       デストラクタ
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
* @note       初期化
* @attention  
******************************************************************************/
void SceneManager::Init()
{
	//初期化
	if (m_pScene)
	{
		switch (m_pScene->m_id)	//どのシーンか？
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
* @note       終了処理
* @attention  
******************************************************************************/
void SceneManager::Uninit()
{
	//終了処理
	if (m_pScene)
	{
		switch (m_pScene->m_id) //どのシーンか？
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
* @note       更新処理
* @attention  
******************************************************************************/
void SceneManager::Update()
{
	//更新
	if (m_pScene)
	{
		switch (m_pScene->m_id)	//どのシーンか？
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
* @note       描画処理
* @attention  
******************************************************************************/
void SceneManager::Draw()
{
	//描画
	if (m_pScene)
	{
		switch (m_pScene->m_id) //どのシーンか？
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
* @note       シーン変更
* @attention  ESceneを参照
******************************************************************************/
void SceneManager::Change(EScene scene)
{
	//現在のシーンを終了
	if (m_pScene) {
		m_pScene->Uninit();
		GUI::Get()->Uninit();
		GLOBALDATA->Uninit();
	}

	//次のシーンを開始
	if (scene)
	{
		m_id = scene;
		m_pScene->Init();
	}
	
}