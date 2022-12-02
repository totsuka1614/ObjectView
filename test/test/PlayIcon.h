#pragma once

#include "UI.h"

class CPlayIcon : public UI
{
public:
	CPlayIcon() {};
	~CPlayIcon() {};

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
private:

};