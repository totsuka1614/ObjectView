/******************************************************************************
* 
* @file      PlayIcon.h
* @brief     �v���C�A�C�R���N���X
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//�C���N���[�h��
#include "UI.h"

class CPlayIcon : public UI
{
public:
	CPlayIcon() {};
	~CPlayIcon() {};

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
private:

};