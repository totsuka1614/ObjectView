#include "Scene.h"
#include "Camera.h"

// �ÓI�����o
SceneManager* SceneManager::m_pScene = nullptr;

SceneManager::SceneManager()
{
	m_id = SCENE_TITLE;
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

// �ύX
void SceneManager::Change(EScene scene)
{
	if (m_pScene) {
		m_pScene->Uninit();
		m_pScene = nullptr;
	}
	
}