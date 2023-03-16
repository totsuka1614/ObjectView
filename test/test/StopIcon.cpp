/******************************************************************************
* 
* @file      StopIcon.cpp
* @brief     停止アイコン
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "stopicon.h"
#include "Input.h"
#include "GlobalData.h"

/******************************************************************************
* 
* @brief      Init
* @param[in]  pos
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       初期化
* @attention  
******************************************************************************/
void CStopIcon::Init(XMFLOAT2 pos)
{
	//Polygon初期化
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;

	//カラーを使用する
	UseColor();

	//設定
	SetTexture("data/Texture/UI/StopIcon.png");
	SetPos(pos.x, pos.y);
	SetSize(256.0f / 2.5f, 256.0f / 2.5f);
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       終了
* @attention  
******************************************************************************/
void CStopIcon::Uninit()
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
void CStopIcon::Update()
{
	//ゲームが始まっていないなら更新しない
	if (!GLOBALDATA->GetStartFlag())
		return;

	//マウスポジション取得
	POINT* pos = CInput::GetMousePosition();

	//停止機能
	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y)))	//マウスが重なっているとき
	{
		//Activeにし、カラーを赤くする
		SetColor(1.0f, 0.0f, 0.0f);
		m_bActive = true;

		//左クリック
		if (CInput::GetMouseButton(MOUSEBUTTON_L))
		{
			//プレイヤー停止
			m_pPlayer->SetStopFlag(true);
		}
		else
		{
			//プレイヤー動作
			m_pPlayer->SetStopFlag(false);
		}
	}
	else
	{
		//NoActive
		m_bActive = false;
		SetColor(1.0f, 1.0f, 1.0f);
		m_pPlayer->SetStopFlag(false);
	}

	//[デバッグ]Sキーで停止
	if (CInput::GetKeyPress(VK_S))
	{
		m_pPlayer->SetStopFlag(true);
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
void CStopIcon::Draw()
{
	UI::Draw();
}