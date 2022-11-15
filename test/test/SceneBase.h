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
#include <list>

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

	template<class T> bool Create(const char* name)
	{
		if (m_pObj.find(name) == m_pObj.end())
		{
			T* pObj = new T;
			m_pObj.insert(std::pair<std::string, GameObjBase*>(name, new GameObj<T>(pObj)));

			return true;
		}

		return false;
	}

	template<class T> T* GetComponent(const char* name)
	{
		std::map<std::string, GameObjBase*>::iterator it = m_pObj.find(name);

		GameObj<T>* ptr = reinterpret_cast<GameObj<T>*>(it->second);
		if (!ptr)
		{

		}

		return ptr->m_pObj;
	}

	void Delete(const char* name)
	{
		std::map<std::string, GameObjBase*>::iterator it = m_pObj.find(name);
		m_pObj.erase(it);
	}

	void DataSave(std::list<std::string> namelist);
	void DataLoad(std::list<std::string>& namelist);
protected :
	std::map<std::string, GameObjBase*> m_pObj;

private :
};

/*
�e���v���[�g�ւ̌l�I����
�E�l�X�ȃI�u�W�F�N�g�̌^����̔z��ɊǗ����邱�Ƃ��ł���
�E��������Unity�Ɏ��Ă�Ɗ�����
�E
*/