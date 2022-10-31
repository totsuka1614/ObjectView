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

class GUI
{
public:
	GUI():m_nGuiCount(0) {};
	~GUI() {};

	void Init();
	void Release();
	void Update();
	void Draw();

	void Create(Model&);
	void CameraCreate();
	static GUI* Get() { return m_pGui; }
private:
	static GUI* m_pGui;

	int m_nGuiCount;

};