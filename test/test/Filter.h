#pragma once

#include "main.h"
#include "Polygon.h"

class CFilter : public CPolygon
{
public:
	CFilter() : m_eType(PIXEL2D),m_nNumber(0),m_fVignette(0.2f),m_fTime(0.0f) {};
	~CFilter() {};

	virtual void Update();
	virtual void Draw();

	
private:
	PSShaderType m_eType;
	int m_nNumber;
	float m_fVignette;
	float m_fTime;
};

