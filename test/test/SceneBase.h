/******************************************************************************
* 
* @file      SceneBase.h
* @brief     シーンベースクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#ifndef __SCENEBASE_H__
#define __SCENEBASE_H__
//インクルード部
#include "main.h"
#include <map>
#include <string>
#include <list>
#include "ObjectBase.h"

//シーンオブジェクトベース
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

	//登録
	template<class T> void Entry(const char* name, T* ptr)
	{
		if (m_pObj.find(name) == m_pObj.end())
		{
			m_pObj.insert(std::pair<std::string,GameObjBase*>( name, new GameObj<T>(ptr)));
		}
	}

	//生成
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

	//取得
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

	//削除
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
テンプレートへの個人的理解
・様々なオブジェクトの型を一つの配列に管理することができた
・書き方はUnityに似てると感じた
・
*/

#endif