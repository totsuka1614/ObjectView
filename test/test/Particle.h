#pragma once

#include "main.h"
#include "Polygon.h"
#include <vector>


class Particle : public CPolygon
{
	// �ϐ�
	typedef struct {
		XMFLOAT2 size;
		int		status;		// ���
		float	px, py;		// �ʒu
		float	vx, vy;		// ���x
		float	ax, ay;		// �����x
		int life;
	} PARTICLE;

public:
	Particle() {};
	~Particle() {};

	virtual void Init(int nSize = 100);
	virtual void Update();
	virtual void Draw();
private:

	std::vector<PARTICLE> m_Particle;			// �p�[�e�B�N���{��
	int m_nInterval;
	int m_nSense;
};
