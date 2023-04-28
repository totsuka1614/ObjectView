/******************************************************************************
* 
* @file      GlobalData.h
* @brief     �O���[�o���f�[�^�N���X
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      �g�������Ă̍����ϐ������[
* @attention 
* 
******************************************************************************/
#pragma once
//�C���N���[�h��
#include "main.h"
//�}�N��
#ifndef GLOBALDATA
#define GLOBALDATA GlobalData::Get()
#endif

class GlobalData
{
public:
	GlobalData();
	~GlobalData();

	void Uninit(void);

	static GlobalData* Get() { return m_pData; }

	void Change(void) { m_bStart = !m_bStart; }
	void Clear(void) { m_bGoal = !m_bGoal; }
	bool GetStartFlag(void) { return m_bStart; }
	bool GetClearFlag(void) { return m_bGoal; }
private:
	static GlobalData* m_pData;

	bool m_bStart;
	bool m_bGoal;
};
