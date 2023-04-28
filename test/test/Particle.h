/******************************************************************************
* 
* @file      Particle.h
* @brief     パーティクルクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//インクルード部
#include "main.h"
#include "Polygon.h"
#include <vector>

class CParticle : public CPolygon
{
	// 変数
	typedef struct {
		XMFLOAT2 size;
		int		status;		// 状態
		float	px, py;		// 位置
		float	vx, vy;		// 速度
		float	ax, ay;		// 加速度
		int life;
	} PARTICLE;

public:
	CParticle() {};
	~CParticle() {};

	virtual void Init(int nSize = 100);
	virtual void Update();
	virtual void Draw();
private:

	std::vector<PARTICLE> m_Particle;			// パーティクル本体
	int m_nInterval;
	int m_nSense;
};
