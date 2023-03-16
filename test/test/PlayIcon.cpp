/******************************************************************************
* 
* @file      PlayIcon.cpp
* @brief     プレイアイコンクラス
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "playicon.h"
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
void CPlayIcon::Init()
{
	//Polygon初期化
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;

	//カラーを使用する
	UseColor();

	//設定
	SetTexture("data/Texture/UI/PlayIcon.png");
	SetPos(550.0f, -300.0f);
	SetSize(256.0f / 2.5f, 256.0f / 2.5f);
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
void CPlayIcon::Uninit()
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
void CPlayIcon::Update()
{
	//マウスポジション取得
	POINT* pos = CInput::GetMousePosition();

	//ゲームスタート処理
	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y)))	//カーソルが重なっているとき
	{
		//Activeにし、カラーを赤くする
		SetColor(1.0f, 0.0f, 0.0f);
		m_bActive = true;

		//左クリック
		if (CInput::GetMouseTrigger(MOUSEBUTTON_L))
		{
			//スタート
			GLOBALDATA->Change();
		}
	}
	else
	{
		//NoActiveにし、色を戻す
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
void CPlayIcon::Draw()
{
	//描画
	if (GLOBALDATA->GetStartFlag())	//ゲームスタートしているか
	{
		//ポーズアイコンに
		SetTexture("data/Texture/UI/PauseIcon.png");
	}
	else
	{
		//プレイアイコンに
		SetTexture("data/Texture/UI/PlayIcon.png");
	}

	UI::Draw();
}