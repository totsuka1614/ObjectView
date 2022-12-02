//=============================================================================
//
// UI ƒNƒ‰ƒX [UI.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================

#include "UI.h"
#include "Input.h"
#include "GlobalData.h"

void UI::Init()
{
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;
}

void UI::Uninit()
{
	CPolygon::Fin();
}

void UI::Update()
{	
}

void UI::Draw()
{
	CPolygon::Draw();
}