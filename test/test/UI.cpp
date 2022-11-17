//=============================================================================
//
// UI ƒNƒ‰ƒX [UI.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================

#include "UI.h"

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
	POINT pos;

	GetCursorPos(&pos);


}

void UI::Draw()
{
	CPolygon::Draw();
}