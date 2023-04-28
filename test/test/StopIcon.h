/******************************************************************************
* 
* @file      StopIcon.h
* @brief     ��~�A�C�R���N���X
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//�C���N���[�h��
#include "UI.h"
#include "Player.h"

class CStopIcon : public UI
{
public:
	CStopIcon() {};
	~CStopIcon() {};

	virtual void Init(XMFLOAT2);
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	void SetPlayer(CPlayer* player) { m_pPlayer = player; }
private:
	CPlayer* m_pPlayer;

};