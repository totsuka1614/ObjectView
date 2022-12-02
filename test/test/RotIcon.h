#pragma once

#include "UI.h"
#include "Player.h"

class CRotIcon : public UI
{
public:
	CRotIcon(){};
	~CRotIcon() {};

	virtual void Init(XMFLOAT2,int nNumber);
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void SetPlayer(CPlayer* player) { m_pPlayer = player; }
private:
	CPlayer* m_pPlayer;
	int m_nNumber;

};