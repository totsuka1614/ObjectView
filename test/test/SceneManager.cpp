#include "SceneManager.h"
#include "Camera.h"

// Ã“Iƒƒ“ƒo
SceneManager* SceneManager::m_pScene = nullptr;

SceneManager::SceneManager()
{
	m_id = SCENE_DEBUG;
	Init();
}

SceneManager::~SceneManager()
{
	SAFE_DELETE(m_pScene);
}

void SceneManager::Init()
{
	m_pScene = new SceneManager;
}

void SceneManager::Uninit()
{
	SAFE_DELETE(m_pScene);
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
	}
	
}