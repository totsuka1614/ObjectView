#include "GlobalData.h"

GlobalData g_data;
GlobalData* GlobalData::m_pData = &g_data;

GlobalData::GlobalData()
{
	m_bStart = false;
	m_bGoal = false;
}

GlobalData::~GlobalData()
{
}

void GlobalData::Uninit()
{
	m_bStart = false;
	m_bGoal = false;

}