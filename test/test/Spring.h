/******************************************************************************
* 
* @file      Spring.h
* @brief     ばねクラス
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      
* @attention 
* 
******************************************************************************/
#pragma once

#include "main.h"
#include "UI.h"

class CSpring : public UI 
{
private:
	struct SPRING {
		int		status;				// ステータス
		bool	onLock;				// 固定フラグ
		bool	onDrag;				// マウスドラッグ中判定フラグ
		XMFLOAT3 position;			// 位置座標 (Position)
		XMFLOAT3 oldposition;			// 位置座標 (Position)
		XMFLOAT3 velocity;			// 速度 (Velocity)
		XMFLOAT3 acceleration;		// 加速度 (Acceleration)
		XMFLOAT3 resultant;			// 合力 (Resultant)
		XMFLOAT3 gravity;			// 重力 (Gravity)
		XMFLOAT3 tension_up;			// 張力 (Tension) 上方向
		XMFLOAT3 tension_down;		// 張力 (Tension) 下方向
	};

public:
	CSpring() {};
	~CSpring() {};

	void Init(void);
	void Update(void);
	void Draw(void);
private:
	SPRING m_Spring;
};