/******************************************************************************
* 
* @file      RotIcon.cpp
* @brief     ��]�A�C�R���N���X
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "Roticon.h"
#include "Input.h"
#include "GlobalData.h"

/******************************************************************************
* �萔�E�}�N����`
/******************************************************************************/
#define ROT_VALUE (2.0f)	//��]��

/******************************************************************************
* 
* @brief      Init
* @param[in]  pos
* @param[in]  nNumber
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ������
* @attention  nNumber 0...����] 1...�E��] 
******************************************************************************/
void CRotIcon::Init(XMFLOAT2 pos,int nNumber)
{
	//Polygon������
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;
	m_nNumber = nNumber;

	//�J���[���g�p���邩
	UseColor();

	//Left or Right
	if(nNumber == 0)
		SetTexture("data/Texture/UI/UI_ArrowLeft.png"); //����]�A�C�R��
	if (nNumber == 1)
		SetTexture("data/Texture/UI/UI_ArrowRight.png"); //�E��]�A�C�R��

	//�ݒ�
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
void CRotIcon::Uninit()
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
void CRotIcon::Update()
{
	//�Q�[�����n�܂��Ă��Ȃ��Ȃ�X�V���Ȃ�
	if (!GLOBALDATA->GetStartFlag())
		return;

	//�}�E�X�|�W�V�����擾
	POINT* pos = CInput::GetMousePosition();

	//��]�@�\
	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y))) //�}�E�X���d�Ȃ��Ă���Ƃ�
	{
		//�Ԃ�����
		SetColor(1.0f, 0.0f, 0.0f);

		//���N���b�N
		if (CInput::GetMouseButton(MOUSEBUTTON_L))
		{
			m_bActive = true;
			if (m_pPlayer)
			{
				//��]
				if (m_nNumber == 0)
					m_pPlayer->GetRotation().y += ROT_VALUE;
				if (m_nNumber == 1)
					m_pPlayer->GetRotation().y -= ROT_VALUE;
			}
		}
		else
			m_bActive = false;
	}
	else
	{
		m_bActive = false;
		SetColor(1.0f, 1.0f, 1.0f);
	}

	//[�f�o�b�O] �L�[����ŉ�]
	if(CInput::GetKeyPress(VK_A) && !m_bActive && m_nNumber == 1)
		m_pPlayer->GetRotation().y -= ROT_VALUE;
	else if(CInput::GetKeyPress(VK_D) && !m_bActive && m_nNumber == 0)
		m_pPlayer->GetRotation().y += ROT_VALUE;


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
void CRotIcon::Draw()
{
	UI::Draw();

}