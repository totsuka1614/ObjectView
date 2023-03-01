//=============================================================================
//
//  ƒNƒ‰ƒX [StopIcon.cpp]
// Author : TOTSUKA KENSUKEStopIcon
//
//=============================================================================

#include "stopicon.h"
#include "Input.h"
#include "GlobalData.h"

void CStopIcon::Init(XMFLOAT2 pos)
{
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;

	UseColor();
	SetTexture("data/Texture/UI/StopIcon.png");
	SetPos(pos.x, pos.y);
	SetSize(256.0f / 2.5f, 256.0f / 2.5f);
}

void CStopIcon::Uninit()
{
	CPolygon::Fin();
}

void CStopIcon::Update()
{
	if (!GLOBALDATA->GetStartFlag())
		return;

	POINT* pos = CInput::GetMousePosition();

	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y)))
	{
		SetColor(1.0f, 0.0f, 0.0f);
		m_bActive = true;
		if (CInput::GetMouseButton(MOUSEBUTTON_L))
		{
			m_pPlayer->SetStopFlag(true);
		}
		else
		{
			m_pPlayer->SetStopFlag(false);
		}
	}
	else
	{
		m_bActive = false;
		SetColor(1.0f, 1.0f, 1.0f);
		m_pPlayer->SetStopFlag(false);
	}

	if (CInput::GetKeyPress(VK_S))
	{
		m_pPlayer->SetStopFlag(true);
	}

}

void CStopIcon::Draw()
{
	UI::Draw();
}