#include "GlobalData.h"

GlobalData g_data;
GlobalData* GlobalData::m_pData = &g_data;

GlobalData::GlobalData()
{
	m_bStart = false;
}

GlobalData::~GlobalData()
{
}
