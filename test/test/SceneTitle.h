/******************************************************************************
* 
* @file      SceneTitle.h
* @brief     �V�[���^�C�g���N���X
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//�C���N���[�h��
#include "main.h"
#include "SceneBase.h"

class CTitle : public SceneBase
{
public:
	CTitle() {};
	~CTitle() {};

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:

};
