/******************************************************************************
* 
* @file      JumpIcon.h
* @brief     ジャンプアイコンクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//インクルード部
#include "main.h"
#include "UI.h"
#include "Player.h"

class CJumpIcon : public UI
{
public:
	CJumpIcon() {};
	~CJumpIcon() {};

	virtual void Init(XMFLOAT2);
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void SetPlayer(CPlayer* player) { m_pPlayer = player; }
private:
	CPlayer* m_pPlayer;
	int m_nNumber;

};

