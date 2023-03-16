/******************************************************************************
* 
* @file      JumpIcon.cpp
* @brief     �W�����v�A�C�R���N���X
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "jumpicon.h"
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
void CJumpIcon::Init(XMFLOAT2 pos)
{
	//Polygon������
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;

	//�J���[���g�p����
	UseColor();

	//�ݒ�
	SetTexture("data/Texture/UI/JumpIcon.png");
	SetPos(pos.x, pos.y);
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
void CJumpIcon::Uninit()
{
	//Polygon�I��
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
void CJumpIcon::Update()
{
	//�Q�[�����n�܂��Ă��Ȃ��Ȃ�X�V���Ȃ�
	if (!GLOBALDATA->GetStartFlag())
		return;

	//�}�E�X�|�W�V�����擾
	POINT* pos = CInput::GetMousePosition();

	//�W�����v�@�\
	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y)))	//�J�[�\�����d�Ȃ��Ă���Ƃ�
	{
		//Active�ɂ��A�J���[��Ԃ�����
		m_bActive = true;
		SetColor(1.0f, 0.0f, 0.0f);
		
		//���N���b�N
		if (CInput::GetMouseTrigger(MOUSEBUTTON_L))
		{
			if (m_pPlayer)
			{
				//�v���C���[�W�����v
				m_pPlayer->Jump(5.0f);
			}
		}
	}
	else
	{
		//NoActive�ɂ��A�F��߂�
		m_bActive = false;
		SetColor(1.0f, 1.0f, 1.0f);
	}

	//[�f�o�b�O]�X�y�[�X�{�^���ŃW�����v
	if(CInput::GetKeyTrigger(VK_SPACE))
		m_pPlayer->Jump(5.0f);


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
void CJumpIcon::Draw()
{
	UI::Draw();
}