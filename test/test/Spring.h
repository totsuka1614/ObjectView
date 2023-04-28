/******************************************************************************
* 
* @file      Spring.h
* @brief     �΂˃N���X
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
		int		status;				// �X�e�[�^�X
		bool	onLock;				// �Œ�t���O
		bool	onDrag;				// �}�E�X�h���b�O������t���O
		XMFLOAT3 position;			// �ʒu���W (Position)
		XMFLOAT3 oldposition;			// �ʒu���W (Position)
		XMFLOAT3 velocity;			// ���x (Velocity)
		XMFLOAT3 acceleration;		// �����x (Acceleration)
		XMFLOAT3 resultant;			// ���� (Resultant)
		XMFLOAT3 gravity;			// �d�� (Gravity)
		XMFLOAT3 tension_up;			// ���� (Tension) �����
		XMFLOAT3 tension_down;		// ���� (Tension) ������
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