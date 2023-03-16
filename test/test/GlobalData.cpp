/******************************************************************************
* 
* @file      GlobalData.cpp
* @brief     �O���[�o���f�[�^
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      �ǂ��ł����������ȕϐ��������[
* @attention 
* 
******************************************************************************/
#include "GlobalData.h"

GlobalData g_data;			//�C���X�^���X
GlobalData* GlobalData::m_pData = &g_data;		//����

/******************************************************************************
* 
* @brief      GlobalData
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/03
* @note       �R���X�g���N�^
* @attention  
******************************************************************************/
GlobalData::GlobalData()
{
	m_bStart = false;
	m_bGoal = false;
}

/******************************************************************************
* 
* @brief      ~GlobalData
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/03
* @note       �f�X�g���N�^
* @attention  
******************************************************************************/
GlobalData::~GlobalData()
{
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/03
* @note       �I������
* @attention  
******************************************************************************/
void GlobalData::Uninit()
{
	m_bStart = false;
	m_bGoal = false;

}