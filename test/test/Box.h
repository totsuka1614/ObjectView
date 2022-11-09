//=============================================================================
//
// 境界ボックス クラス [Box.h]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#pragma once
#include "main.h"
#include "Mesh.h"

class Box : public CMesh
{
public:
	Box()
	{
		m_vTarget = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
		SetName("BoxCollider");
	};
	~Box()
	{
	};

	void Init(XMFLOAT3 vSize);

	virtual void Update();
	virtual void Draw();

	XMFLOAT3& GetMove() { return m_vMove; }

	void SetTarget(XMFLOAT3& pos) { m_vTarget = pos; }

private:
	XMFLOAT3 m_vTarget;
	XMFLOAT3 m_vMove;
};