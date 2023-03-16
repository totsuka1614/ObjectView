/******************************************************************************
* 
* @file      PlayIcon.cpp
* @brief     �v���C�A�C�R���N���X
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "playicon.h"
#include "Input.h"
#include "GlobalData.h"

/******************************************************************************
* 
* @brief      Init
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ������
* @attention  
******************************************************************************/
void CPlayIcon::Init()
{
	//Polygon������
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;

	//�J���[���g�p����
	UseColor();

	//�ݒ�
	SetTexture("data/Texture/UI/PlayIcon.png");
	SetPos(550.0f, -300.0f);
	SetSize(256.0f / 2.5f, 256.0f / 2.5f);
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �I������
* @attention  
******************************************************************************/
void CPlayIcon::Uninit()
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
void CPlayIcon::Update()
{
	//�}�E�X�|�W�V�����擾
	POINT* pos = CInput::GetMousePosition();

	//�Q�[���X�^�[�g����
	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y)))	//�J�[�\�����d�Ȃ��Ă���Ƃ�
	{
		//Active�ɂ��A�J���[��Ԃ�����
		SetColor(1.0f, 0.0f, 0.0f);
		m_bActive = true;

		//���N���b�N
		if (CInput::GetMouseTrigger(MOUSEBUTTON_L))
		{
			//�X�^�[�g
			GLOBALDATA->Change();
		}
	}
	else
	{
		//NoActive�ɂ��A�F��߂�
		m_bActive = false;
		SetColor(1.0f, 1.0f, 1.0f);
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
void CPlayIcon::Draw()
{
	//�`��
	if (GLOBALDATA->GetStartFlag())	//�Q�[���X�^�[�g���Ă��邩
	{
		//�|�[�Y�A�C�R����
		SetTexture("data/Texture/UI/PauseIcon.png");
	}
	else
	{
		//�v���C�A�C�R����
		SetTexture("data/Texture/UI/PlayIcon.png");
	}

	UI::Draw();
}