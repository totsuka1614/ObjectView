#include "SceneManager.h"
#include "Camera.h"

SceneManager g_SceneManager;

// Ã“Iƒƒ“ƒo
SceneManager* SceneManager::m_pScene = &g_SceneManager;

SceneManager::SceneManager()
{
	m_id = SCENE_DEBUG;
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
			break;
		case SCENE_GAME:
			break;
		case SCENE_DEBUG:
			m_pDebug->Draw();
			break;
		}
	}
}


// •ÏX
void SceneManager::Change(EScene scene)
{
	if (m_pScene) {
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	if (scene)
	{
		m_id = scene;
		m_pScene->Init();
	}
	
}