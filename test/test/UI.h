/******************************************************************************
* 
* @file      UI.h
* @brief     UIクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//インクルード部
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
