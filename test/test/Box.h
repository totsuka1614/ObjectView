/******************************************************************************
* 
* @file      Box.h
* @brief     ボックスクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
#include "main.h"
#include "Mesh.h"

class CBox : public CMesh
{
public:
	//コンストラクタ
	CBox()
	{
		m_vTarget = new TARGET_TRANSFORM;
		m_vTarget->pos = new XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vTarget->scale = new XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vTarget->deglee = new XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vMove.pos = new XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vMove.scale = new XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vMove.deglee = new XMFLOAT3(0.0f, 0.0f, 0.0f);
	};
	//デストラクタ
	~CBox()
	{
	};

	void Init(XMFLOAT3 vSize);
	virtual void Update();
	virtual void Draw();
	virtual void ColliderDraw();

	void SetTarget(TARGET_TRANSFORM& pos) { m_vTarget = &pos; }

private:
	TARGET_TRANSFORM* m_vTarget;
	TARGET_TRANSFORM m_vMove;
};