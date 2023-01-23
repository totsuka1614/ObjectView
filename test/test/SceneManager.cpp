#include "SceneManager.h"
#include "Camera.h"
#include "GUI.h"
#include "GlobalData.h"

SceneManager g_SceneManager;

// 静的メンバ
SceneManager* SceneManager::m_pScene = &g_SceneManager;

SceneManager::SceneManager()
{
	m_id = SCENE_TITLE;
}

SceneManager::~SceneManager()
{
	//SAFE_DELETE(m_pScene);
}

void SceneManager::Init()
{

	if (m_pScene)
	{
		switch (m_pScene->m_id)
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

void SceneManager::Uninit()
{
	if (m_pScene)
	{
		switch (m_pScene->m_id)
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

void SceneManager::Update()
{
	if (m_pScene)
	{
		switch (m_pScene->m_id)
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

void SceneManager::Draw()
{
	if (m_pScene)
	{
		switch (m_pScene->m_id)
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


// 変更
void SceneManager::Change(EScene scene)
{
	if (m_pScene) {
		m_pScene->Uninit();
		GUI::Get()->Uninit();
		GLOBALDATA->Uninit();
	}

	if (scene)
	{
		m_id = scene;
		m_pScene->Init();
	}
	
}