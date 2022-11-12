//=============================================================================
//
// �V�[��Base �N���X��` [SceneBase.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once

#include "main.h"
#include <map>
#include <string>

class GameObjBase
{
public:
	virtual ~GameObjBase() {};

private:

};

template<class T>
class GameObj : public GameObjBase
{
public:
	GameObj(T* ptr) : m_pObj(ptr) {}
	virtual ~GameObj() {
		delete m_pObj;
	}
	T* m_pObj;
};

class SceneBase
{
public:
	SceneBase() {};
	~SceneBase() {};

	virtual void Update() {};
	virtual void Draw() {};

	template<class T> void Entry(const char* name, T* ptr)
	{
		if (m_pObj.find(name) == m_pObj.end())
		{
			m_pObj.insert(std::pair<std::string,GameObjBase*>( name, new GameObj<T>(ptr)));
		}
	}

	template<class T> void Create(const char* name)
	{
		if (m_pObj.find(name) == m_pObj.end())
		{
			T* pObj = new T;
			m_pObj.insert(std::pair<std::string, GameObjBase*>(name, new GameObj<T>(pObj)));
		}

	}

	template<class T> T* Get(const char* name)
	{
		std::map<std::string, GameObjBase*>::iterator it = m_pObj.find(name);

		GameObj<T>* ptr = reinterpret_cast<GameObj<T>*>(it->second);
		if (!ptr)
		{

		}

		return ptr->m_pObj;
	}

protected :
	std::map<std::string, GameObjBase*> m_pObj;
};

/*
�e���v���[�g�ւ̌l�I����
�E�l�X�ȃI�u�W�F�N�g�̌^����̔z��ɊǗ����邱�Ƃ��ł���
�E��������Unity�Ɏ��Ă�Ɗ�����
�E
*/

//m_pDebug.Entry<Player>("Player1",obj);