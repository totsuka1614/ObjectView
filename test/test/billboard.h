#pragma once

#include "main.h"
#include "Polygon.h"

class CBillboard : public CPolygon
{
public:
	CBillboard() {};
	~CBillboard() {};

	virtual void Init();
	virtual void Draw();

private:

};