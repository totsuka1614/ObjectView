//=============================================================================
//
// Player クラス定義 [Player.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"
#include "Box.h"

class CPlayer : public Model
{
public:
	CPlayer()
	{
		SetName("Player");
		m_vMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
	virtual ~CPlayer() {}

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetStopFlag(bool stop) { m_bStop = stop; }
	void Jump(float power);


private:

	XMFLOAT3 m_vMove;
	XMFLOAT3 m_vVel;
	XMFLOAT3 m_vStartPos;
	XMFLOAT3 m_vStartRot;
	bool m_bJump;
	bool m_bStop;
	bool m_bStart;
	ID3D11ShaderResourceView* m_pNormal;
};
