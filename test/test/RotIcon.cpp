//=============================================================================
//
// PlayIcon ƒNƒ‰ƒX [PlayIcon.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================

#include "Roticon.h"
#include "Input.h"
#include "GlobalData.h"

void CRotIcon::Init(XMFLOAT2 pos,int nNumber)
{
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;
	m_nNumber = nNumber;
	UseColor();

	if(nNumber == 0)
		SetTexture("data/Texture/UI_ArrowLeft.png");
	if (nNumber == 1)
		SetTexture("data/Texture/UI_ArrowRight.png");

	SetPos(pos.x, pos.y);
	SetSize(256.0f / 2.5f, 256.0f / 2.5f);
}

void CRotIcon::Uninit()
{
	CPolygon::Fin();
}

void CRotIcon::Update()
{
	if (!GlobalData::Get()->GetStartFlag())
		return;

	POINT* pos = CInput::GetMousePosition();

	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y)))
	{
		SetColor(1.0f, 0.0f, 0.0f);
		m_bActive = true;
		if (CInput::GetMouseButton(MOUSEBUTTON_L))
		{
			if (m_pPlayer)
			{
				if(m_nNumber == 0)
					m_pPlayer->GetRotation().y += 2.0f;
				if(m_nNumber == 1)
					m_pPlayer->GetRotation().y -= 2.0f;
			}
		}
	}
	else
	{
		m_bActive = false;
		SetColor(1.0f, 1.0f, 1.0f);
	}


}

void CRotIcon::Draw()
{
	UI::Draw();

}