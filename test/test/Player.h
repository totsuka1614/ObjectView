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

	void SetMove(XMFLOAT3 move) { m_vMove.x += move.x; m_vMove.y += move.y; m_vMove.z += move.z; }
	void Jump(float power);


private:
	Box* m_Box;

	XMFLOAT3 m_vMove;
	XMFLOAT3 m_vVel;
	bool m_bJump;
};
