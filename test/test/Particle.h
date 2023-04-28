/******************************************************************************
* 
* @file      Particle.h
* @brief     �p�[�e�B�N���N���X
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//�C���N���[�h��
#include "main.h"
#include "Polygon.h"
#include <vector>

class CParticle : public CPolygon
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
	CParticle() {};
	~CParticle() {};

	virtual void Init(int nSize = 100);
	virtual void Update();
	virtual void Draw();
private:

	std::vector<PARTICLE> m_Particle;			// �p�[�e�B�N���{��
	int m_nInterval;
	int m_nSense;
};
