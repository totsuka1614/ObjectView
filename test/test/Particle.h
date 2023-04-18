#pragma once

#include "main.h"
#include "Polygon.h"
#include <vector>


class Particle : public CPolygon
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
	Particle() {};
	~Particle() {};

	virtual void Init(int nSize = 100);
	virtual void Update();
	virtual void Draw();
private:

	std::vector<PARTICLE> m_Particle;			// パーティクル本体
	int m_nInterval;
	int m_nSense;
};
