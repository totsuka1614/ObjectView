#pragma once

#include "main.h"
#include "Polygon.h"

class CTrail : public CPolygon 
{
public:
	CTrail() {};
	~CTrail() {};

	void Init(void);
	void Update(void);
	void Draw(void);
private:
	int m_nInterval;
	POINT m_OldMouse;
};
