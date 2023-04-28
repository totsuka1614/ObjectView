/******************************************************************************
* 
* @file      SceneBase.h
* @brief     �V�[���x�[�X�N���X
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#ifndef __SCENEBASE_H__
#define __SCENEBASE_H__
//�C���N���[�h��
#include "main.h"
#include <map>
#include <string>
#include <list>
#include "ObjectBase.h"

//�V�[���I�u�W�F�N�g�x�[�X
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
	SceneBase() {
	};
	~SceneBase() {};

	virtual void Update() {};
	virtual void Draw() {};

	//�o�^
	template<class T> void Entry(const char* name, T* ptr)
	{
		if (m_pObj.find(name) == m_pObj.end())
		{
			m_pObj.insert(std::pair<std::string,GameObjBase*>( name, new GameObj<T>(ptr)));
		}
	}

	//����
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

	//�擾
	template<class T> T* GetComponent(const char* name)
	{
		std::map<std::string, GameObjBase*>::iterator it = m_pObj.find(name);

		if (it == m_pObj.end())
		{

		}
		else
		{
			GameObj<T>* ptr = reinterpret_cast<GameObj<T>*>(it->second);
			return ptr->m_pObj;
		}
		return NULL;
	}

	//�폜
	void Delete(const char* name)
	{
		std::map<std::string, GameObjBase*>::iterator it = m_pObj.find(name);
		if (it != m_pObj.end())
			m_pObj.erase(it);
	}

	void DataSave(std::list<std::string> namelist);
	void DataLoad(std::list<std::string>& namelist);

	std::map<std::string, GameObjBase*> GetObj() { return m_pObj; }
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

#endif