//=============================================================================
//
// PlayIcon ƒNƒ‰ƒX [PlayIcon.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================

#include "playicon.h"
#include "Input.h"
#include "GlobalData.h"

void CPlayIcon::Init()
{
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;

	UseColor();
	SetTexture("data/Texture/PlayIcon.png");
	SetPos(550.0f, -300.0f);
	SetSize(256.0f / 2.5f, 256.0f / 2.5f);
}

void CPlayIcon::Uninit()
{
	CPolygon::Fin();
}

void CPlayIcon::Update()
{
	POINT* pos = CInput::GetMousePosition();

	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y)))
	{
		SetColor(1.0f, 0.0f, 0.0f);
		m_bActive = true;
		if (CInput::GetMouseTrigger(MOUSEBUTTON_L))
		{
			GlobalData::Get()->Change();
		}
	}
	else
	{
		m_bActive = false;
		SetColor(1.0f, 1.0f, 1.0f);
	}


}

void CPlayIcon::Draw()
{
	if (GlobalData::Get()->GetStartFlag())
	{
		SetTexture("data/Texture/StopIcon.png");
	}
	else
	{
		SetTexture("data/Texture/PlayIcon.png");
	}

	UI::Draw();
}