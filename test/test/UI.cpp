/******************************************************************************
* 
* @file      UI.cpp
* @brief     UIクラス
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "UI.h"
#include "Input.h"
#include "GlobalData.h"

/******************************************************************************
* 
* @brief      Init
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       初期化
* @attention  
******************************************************************************/
void UI::Init()
{
	//Polygon初期化
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       終了処理
* @attention  
******************************************************************************/
void UI::Uninit()
{
	CPolygon::Fin();
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       更新
* @attention  
******************************************************************************/
void UI::Update()
{	
	//マウスポジション取得
	POINT* pos = CInput::GetMousePosition();

	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y))) //マウスが重なっているとき
	{
		//Activeにし、カラーを赤くする
		SetColor(1.0f, 0.0f, 0.0f);
		m_bActive = true;
	}
	else
	{
		//NoActiveにし、カラーを戻す
		m_bActive = false;
		SetColor(1.0f, 1.0f, 1.0f);
	}
}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       描画
* @attention  
******************************************************************************/
void UI::Draw()
{
	CPolygon::Draw();
}

/******************************************************************************
* 
* @brief      CollisionToPoint
* @param[in]  pos
* @return     bool
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       2Dと2Dの当たり判定(当たっている場合true)
* @attention  
******************************************************************************/
bool UI::CollisionToPoint(XMFLOAT2 pos)
{
	//当たり判定
	if (m_vPos.x - m_vScale.x / 2 < pos.x && m_vPos.x + m_vScale.x / 2 > pos.x) //横軸
	{
		if (m_vPos.y - m_vScale.y / 2 < pos.y && m_vPos.y + m_vScale.y / 2 > pos.y) //縦軸
		{
			return true;
		}
	}

	return false;
}