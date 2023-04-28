/******************************************************************************
* 
* @file      GUI.h
* @brief     IMGUIクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//インクルード部
#include "main.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "window.h"
#include "BackBuffer.h"
#include "FBX.h"
#include "model.h"
#include "Camera.h"
#include "Mesh.h"
#include "Box.h"
#include "Polygon.h"
#include <list>
#include "ObjectBase.h"

//マクロ
#ifndef IMGUI
#define IMGUI GUI::Get()
#endif

//オブジェクト生成構造体
typedef struct
{
	ObjectType type;
	char cName[128] = "default";
	char cPath[256] = "data/model/.fbx";
	bool bCreate;
}CREATE_OBJECT;

class GUI
{
public:
	GUI():m_nGuiCount(0){};
	~GUI() {};

	void Init();
	void Release();
	void Update();
	void Draw();

	void Entry(CObjectBase& model) { m_ObjectList.push_back(&model); };
	void Uninit(void);
	void Display();
	CREATE_OBJECT DebugDisplay(void);
	void EnvironmentCreate();
	static GUI* Get() { return m_pGui; }
	std::list<CObjectBase*> GetObjectList() { return m_ObjectList; }

private:
	void ListDisplay(void);
	void ObjectDisplay(void);

	static GUI* m_pGui;

	int m_nGuiCount;
	bool m_bDisplay;

	std::list<CObjectBase*> m_ObjectList;
};