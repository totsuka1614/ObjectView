/******************************************************************************
* 
* @file      Trail.h
* @brief     追尾パーティクルクラス
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//インクルード部
#include "main.h"
#include "Polygon.h"

#define TRAIL_MAX		100					// パーティクルの数

class CTrail : public CPolygon 
{
private:
	typedef struct {
		XMFLOAT2 size;
		int		status;		// 状態
		float	px, py;		// 位置
		float	vx, vy;		// 速度
		float	ax, ay;		// 加速度
		int life;
	} TRAIL;
public:
	CTrail() {};
	~CTrail() {};

	void Init(void);
	void Update(void);
	void Draw(void);
private:
	int m_nInterval;
	POINT m_OldMouse; 
	TRAIL m_Trail[TRAIL_MAX];			// パーティクル本体
};
