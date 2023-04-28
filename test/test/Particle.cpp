/******************************************************************************
* 
* @file      Particle.cpp
* @brief     �p�[�e�B�N���N���X
* @author    Totsuka Kensuke
* @date      2023/04/18
* @note      �}�E�X�ɒǏ]����
* @attention 
* 
******************************************************************************/

// �C���N���[�h�t�@�C��
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "Particle.h"
#include "window.h"
#include "Input.h"

/******************************************************************************
* 
* @brief      Init
* @param[in]  nSize
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       ������
* @attention  �����Ő���ݒ�
* 
******************************************************************************/
void CParticle::Init(int nSize)
{
	//�p�[�e�B�N�����ݒ�
	m_Particle.resize(nSize);

	//������
	for (std::vector<PARTICLE>::iterator part = m_Particle.begin(); part != m_Particle.end(); part++)
	{
		part->status = 0;	// �X�e�[�^�X��������
		part->life = 0;	// �X�e�[�^�X��������
		part->size = XMFLOAT2(9.0f, 9.0f);	// �X�e�[�^�X��������
	}
	SetPos(0.0f, 0.0f);
	SetSize(9.0f, 9.0f);
	SetColor(1.0f, 0.0f, 0.0f);
	m_nInterval = 0;
	m_nSense = 3;
	//�|���S��������
	CPolygon::Init();

	//�p�[�e�B�N���e�N�X�`���ݒ�
	SetTexture("data/Texture/Particle001.png");
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       �X�V����
* @attention  
* 
******************************************************************************/
void CParticle::Update()
{
	//�p�[�e�B�N�������Ԋu
	if (m_nInterval < m_nSense)
		m_nInterval++;
	else
		m_nInterval = 0;

	//�����t���O
	bool	bBorn;
	bBorn = false;

	//�}�E�X���W�擾
	POINT* mouse = CInput::GetMousePosition();

	mouse->x -= SCREEN_CENTER_X;
	mouse->y *= -1;
	mouse->y += SCREEN_CENTER_Y;

	//�p�[�e�B�N������
	for (std::vector<PARTICLE>::iterator part = m_Particle.begin(); part != m_Particle.end(); part++) {
		switch (part->status) {
		case 0:					// �ҋ@���
			if (!bBorn && m_nInterval >= m_nSense) {
				bBorn = true;
				part->status = 1;
			}
			break;
		case 1:
			//�����_������
			part->px = (float)mouse->x;
			part->py = (float)mouse->y;
			part->vx = (float)(rand() % 6 - 2.5f);
			part->vy = (float)(rand() % 7 - 2.5f);
			part->ax = 0.0f;
			part->ay = 0.0f;
			part->status = 2;
			// THRU
		case 2:
			//�����_���ɓ���
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

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       �`�揈��
* @attention  
* 
******************************************************************************/
void CParticle::Draw()
{
	//�A���t�@�u�����h�ݒ�
	BACKBUFFER->SetBlendState(BS_ALPHABLEND);

	//�`��
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