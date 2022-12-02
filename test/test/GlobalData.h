#pragma once

#include "main.h"

class GlobalData
{
public:
	GlobalData();
	~GlobalData();

	static GlobalData* Get() { return m_pData; }

	void Change() { m_bStart = !m_bStart; }
	bool GetStartFlag() { return m_bStart; }
private:
	static GlobalData* m_pData;

	bool m_bStart;
};
