/******************************************************************************
* 
* @file      StopIcon.cpp
* @brief     ��~�A�C�R��
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "stopicon.h"
#include "Input.h"
#include "GlobalData.h"

/******************************************************************************
* 
* @brief      Init
* @param[in]  pos
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ������
* @attention  
******************************************************************************/
void CStopIcon::Init(XMFLOAT2 pos)
{
	//Polygon������
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;

	//�J���[���g�p����
	UseColor();

	//�ݒ�
	SetTexture("data/Texture/UI/StopIcon.png");
	SetPos(pos.x, pos.y);
	SetSize(256.0f / 2.5f, 256.0f / 2.5f);
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �I��
* @attention  
******************************************************************************/
void CStopIcon::Uninit()
{
	CPolygon::Fin();
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �X�V
* @attention  
******************************************************************************/
void CStopIcon::Update()
{
	//�Q�[�����n�܂��Ă��Ȃ��Ȃ�X�V���Ȃ�
	if (!GLOBALDATA->GetStartFlag())
		return;

	//�}�E�X�|�W�V�����擾
	POINT* pos = CInput::GetMousePosition();

	//��~�@�\
	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y)))	//�}�E�X���d�Ȃ��Ă���Ƃ�
	{
		//Active�ɂ��A�J���[��Ԃ�����
		SetColor(1.0f, 0.0f, 0.0f);
		m_bActive = true;

		//���N���b�N
		if (CInput::GetMouseButton(MOUSEBUTTON_L))
		{
			//�v���C���[��~
			m_pPlayer->SetStopFlag(true);
		}
		else
		{
			//�v���C���[����
			m_pPlayer->SetStopFlag(false);
		}
	}
	else
	{
		//NoActive
		m_bActive = false;
		SetColor(1.0f, 1.0f, 1.0f);
		m_pPlayer->SetStopFlag(false);
	}

	//[�f�o�b�O]S�L�[�Œ�~
	if (CInput::GetKeyPress(VK_S))
	{
		m_pPlayer->SetStopFlag(true);
	}

}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �`��
* @attention  
******************************************************************************/
void CStopIcon::Draw()
{
	UI::Draw();
}