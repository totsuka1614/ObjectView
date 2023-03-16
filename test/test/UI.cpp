/******************************************************************************
* 
* @file      UI.cpp
* @brief     UI�N���X
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "UI.h"
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
void UI::Init()
{
	//Polygon������
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;
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
void UI::Uninit()
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
void UI::Update()
{	
	//�}�E�X�|�W�V�����擾
	POINT* pos = CInput::GetMousePosition();

	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y))) //�}�E�X���d�Ȃ��Ă���Ƃ�
	{
		//Active�ɂ��A�J���[��Ԃ�����
		SetColor(1.0f, 0.0f, 0.0f);
		m_bActive = true;
	}
	else
	{
		//NoActive�ɂ��A�J���[��߂�
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
void UI::Draw()
{
	CPolygon::Draw();
}

/******************************************************************************
* 
* @brief      CollisionToPoint
* @param[in]  pos
* @return     bool
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       2D��2D�̓����蔻��(�������Ă���ꍇtrue)
* @attention  
******************************************************************************/
bool UI::CollisionToPoint(XMFLOAT2 pos)
{
	//�����蔻��
	if (m_vPos.x - m_vScale.x / 2 < pos.x && m_vPos.x + m_vScale.x / 2 > pos.x) //����
	{
		if (m_vPos.y - m_vScale.y / 2 < pos.y && m_vPos.y + m_vScale.y / 2 > pos.y) //�c��
		{
			return true;
		}
	}

	return false;
}