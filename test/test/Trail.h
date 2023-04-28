/******************************************************************************
* 
* @file      Trail.h
* @brief     �ǔ��p�[�e�B�N���N���X
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//�C���N���[�h��
#include "main.h"
#include "Polygon.h"

#define TRAIL_MAX		100					// �p�[�e�B�N���̐�

class CTrail : public CPolygon 
{
private:
	typedef struct {
		XMFLOAT2 size;
		int		status;		// ���
		float	px, py;		// �ʒu
		float	vx, vy;		// ���x
		float	ax, ay;		// �����x
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
	TRAIL m_Trail[TRAIL_MAX];			// �p�[�e�B�N���{��
};
