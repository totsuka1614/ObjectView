//=============================================================================
//
// PlayIcon ƒNƒ‰ƒX [PlayIcon.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================

#include "jumpicon.h"
#include "Input.h"
#include "GlobalData.h"

void CJumpIcon::Init(XMFLOAT2 pos)
{
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;
	UseColor();

	SetTexture("data/Texture/UI/JumpIcon.png");
	SetPos(pos.x, pos.y);
	SetSize(256.0f / 2.5f, 256.0f / 2.5f);
}

void CJumpIcon::Uninit()
{
	CPolygon::Fin();
}

void CJumpIcon::Update()
{
	if (!GLOBALDATA->GetStartFlag())
		return;

	POINT* pos = CInput::GetMousePosition();

	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y)))
	{
		SetColor(1.0f, 0.0f, 0.0f);
		m_bActive = true;
		if (CInput::GetMouseTrigger(MOUSEBUTTON_L))
		{
			if (m_pPlayer)
			{
				m_pPlayer->Jump(5.0f);
			}
		}
	}
	else
	{
		m_bActive = false;
		SetColor(1.0f, 1.0f, 1.0f);
	}

	if(CInput::GetKeyTrigger(VK_SPACE))
		m_pPlayer->Jump(5.0f);


}

void CJumpIcon::Draw()
{
	UI::Draw();

}