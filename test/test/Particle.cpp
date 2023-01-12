//=======================================================================================
//
//	�p�[�e�B�N���@�����̕\��
//
//=======================================================================================

// �C���N���[�h�t�@�C��
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "Particle.h"
#include "window.h"
#include "Input.h"



void Particle::Init(int nSize)
{
	m_Particle.resize(nSize);

	for (std::vector<PARTICLE>::iterator part = m_Particle.begin(); part != m_Particle.end(); part++)
	{
		part->status = 0;	// �X�e�[�^�X��������
		part->life = 0;	// �X�e�[�^�X��������
		part->size = XMFLOAT2(9.0f, 9.0f);	// �X�e�[�^�X��������
	}

	CPolygon::Init();

	SetTexture("data/Texture/Particle001.png");

	SetPos(0.0f, 0.0f);
	SetSize(9.0f, 9.0f);
	SetColor(1.0f, 0.0f, 0.0f);
	m_nInterval = 0;
}

//---------------------------------------------------------------------------------------
//	�p�[�e�B�N���{�̏���
//---------------------------------------------------------------------------------------
void Particle::Update()
{

	if (m_nInterval < 3)
	{
		m_nInterval++;
	}
	else
	{
		m_nInterval = 0;
	}
	int		i;
	bool	bBorn;

	bBorn = false;

	POINT* mouse = CInput::GetMousePosition();

	mouse->x -= SCREEN_CENTER_X;
	mouse->y *= -1;
	mouse->y += SCREEN_CENTER_Y;

	for (std::vector<PARTICLE>::iterator part = m_Particle.begin(); part != m_Particle.end(); part++) {
		switch (part->status) {
		case 0:					// �ҋ@���
			if (!bBorn && m_nInterval >=3) {
				bBorn = true;
				part->status = 1;
			}
			break;
		case 1:
			part->px = (float)mouse->x;
			part->py = (float)mouse->y;
			part->vx = (float)(rand() % 6 - 2.5f);
			part->vy = (float)(rand() % 7 - 2.5f);
			part->ax = 0.0f;
			part->ay = 0.0f;
			part->status = 2;
			// THRU
		case 2:

			part->vx += part->ax;
			part->vy += part->ay;

			part->px += part->vx;
			part->py += part->vy;
			
			part->life++;

			if (part->life >= 40) {
				part->size.x -= 0.5f;
				part->size.y -= 0.5f;
				if (part->size.x <= 0.0f)
				{
					part->status = 0;
					part->life = 0;
					part->size.x = 9.0f;
					part->size.y = 9.0f;
				}
			}
			break;
		}
	}
}

//---------------------------------------------------------------------------------------
//	�p�[�e�B�N���{�̕`��
//---------------------------------------------------------------------------------------
void Particle::Draw()
{
	BACKBUFFER->SetBlendState(BS_ALPHABLEND);
	for (std::vector<PARTICLE>::iterator part = m_Particle.begin(); part != m_Particle.end(); part++) {
		if (part->status == 2) {
			m_vPos.x = part->px;
			m_vPos.y = part->py;
			m_vScale.x = part->size.x;
			m_vScale.y = part->size.y;
			CPolygon::Draw();
		}
	}
}