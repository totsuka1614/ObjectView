//=============================================================================
//
// シーンデバッグ クラス定義 [SceneDebug.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once

#include "main.h"
#include "Polygon.h"

class UI : public CPolygon
{
public:
	UI(const char* path) { SetTexture(path); };
	UI() {};
	~UI() {};

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	bool CollisionToPoint(XMFLOAT2 pos);

protected:
	bool m_bEvent;
private:
};
