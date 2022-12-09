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
	POINT* pos = CInput::GetMousePosition();

	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y)))
	{
		SetColor(1.0f, 0.0f, 0.0f);
		m_bActive = true;
	}
	else
	{
		m_bActive = false;
		SetColor(1.0f, 1.0f, 1.0f);
	}
}

void UI::Draw()
{
	CPolygon::Draw();
}

bool UI::CollisionToPoint(XMFLOAT2 pos)
{
	if (m_vPos.x - m_vScale.x / 2 < pos.x && m_vPos.x + m_vScale.x / 2 > pos.x)
	{
		if (m_vPos.y - m_vScale.y / 2 < pos.y && m_vPos.y + m_vScale.y / 2 > pos.y)
		{
			return true;
		}
	}

	return false;
}