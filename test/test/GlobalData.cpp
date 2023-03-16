/******************************************************************************
* 
* @file      GlobalData.cpp
* @brief     グローバルデータ
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      どこでもつかいそうな変数等を収納
* @attention 
* 
******************************************************************************/
#include "GlobalData.h"

GlobalData g_data;			//インスタンス
GlobalData* GlobalData::m_pData = &g_data;		//現在

/******************************************************************************
* 
* @brief      GlobalData
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/03
* @note       コンストラクタ
* @attention  
******************************************************************************/
GlobalData::GlobalData()
{
	m_bStart = false;
	m_bGoal = false;
}

/******************************************************************************
* 
* @brief      ~GlobalData
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/03
* @note       デストラクタ
* @attention  
******************************************************************************/
GlobalData::~GlobalData()
{
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/03
* @note       終了処理
* @attention  
******************************************************************************/
void GlobalData::Uninit()
{
	m_bStart = false;
	m_bGoal = false;

}