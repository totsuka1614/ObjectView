//=============================================================================
//
// GUI ƒNƒ‰ƒX’è‹` [GUI.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once

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

#ifndef IMGUI
#define IMGUI GUI::Get()
#endif

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
	GUI():m_nGuiCount(0) 
	{

	};
	~GUI() {};

	void Init();
	void Release();
	void Update();
	void Draw();

	void Entry(ObjectBase& model) { m_ObjectList.push_back(&model); };
	void Uninit(void);
	void Display();
	CREATE_OBJECT DebugDisplay(void);
	void EnvironmentCreate();
	static GUI* Get() { return m_pGui; }
	std::list<ObjectBase*> GetObjectList() { return m_ObjectList; }

private:
	void ListDisplay(void);
	void ObjectDisplay(void);


	static GUI* m_pGui;

	int m_nGuiCount;
	bool m_bDisplay;

	std::list<ObjectBase*> m_ObjectList;
};