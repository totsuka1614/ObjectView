//=============================================================================
//
// シーンBase クラス定義 [SceneBase.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once

#include "main.h"
#include <map>
#include <string>

template<class T>
class GameObj
{
public:
	GameObj(T* ptr) : m_pObj(ptr) {}
	virtual ~GameObj() {
		delete m_pObj;
	}
	T* m_pObj;
};

template<class T>
class SceneBase
{
public:
	SceneBase() {};
	~SceneBase() {};

	virtual void Update();
	virtual void Draw();

	template<class T> void Entry(const char* name, T* ptr) 
	{
		if (m_pObj.find(name) == m_pObj.end())
		{
			m_pObj.insert(std::pair<std::string, GameObj*>(name, new GameObj<T>(ptr)));
		}
	};
private:
	std::map<std::string, GameObj*> m_pObj;
};