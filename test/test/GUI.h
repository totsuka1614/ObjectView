//=============================================================================
//
// GUI ƒNƒ‰ƒX’è‹` [GUI.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once

#include "main.h"
#include "GUI.h"
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
#include <list>

typedef struct
{
	ObjectType type;
	char cName[128] = "default";
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

	void Entry(Model& model) { m_ModelList.push_back(&model); };
	void Entry(CMesh& mesh) { m_MeshList.push_back(&mesh); };

	void Display();
	CREATE_OBJECT DebugDisplay(void);
	void CameraCreate();
	static GUI* Get() { return m_pGui; }

private:
	void ListDisplay(void);
	void ObjectDisplay(void);


	static GUI* m_pGui;

	int m_nGuiCount;

	std::list<CMesh*> m_MeshList;
	std::list<Model*> m_ModelList;
};