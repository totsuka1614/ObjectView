//=============================================================================
//
// Player ƒNƒ‰ƒX’è‹` [Player.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"

class CPlayer : public Model
{
public:
	CPlayer()
	{
		SetName("Player");
	}
	virtual ~CPlayer() {}

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	Box* m_Box;
};
