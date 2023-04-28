/******************************************************************************
* 
* @file      SplitCamera.h
* @brief     �����J�����N���X
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//�C���N���[�h��
#include "main.h"
#include "Camera.h"

//�������[�h��
enum SplitMode
{
	NOW_VIEW,
	SIDE_VIEW,
	FRONT_VIEW,
	UP_VIEW,

	MAX_VIEW
};

class CCameraSplit : public CCamera
{
public:
	CCameraSplit() {};
	~CCameraSplit() {};

	void Init();
	void Update() {};
	void SetMode(SplitMode mode);

private:
	SplitMode m_eMode;

};