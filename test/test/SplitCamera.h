/******************************************************************************
* 
* @file      SplitCamera.h
* @brief     分割カメラクラス
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//インクルード部
#include "main.h"
#include "Camera.h"

//分割モード列挙
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