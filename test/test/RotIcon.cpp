/******************************************************************************
* 
* @file      RotIcon.cpp
* @brief     回転アイコンクラス
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "Roticon.h"
#include "Input.h"
#include "GlobalData.h"

/******************************************************************************
* 定数・マクロ定義
/******************************************************************************/
#define ROT_VALUE (2.0f)	//回転量

/******************************************************************************
* 
* @brief      Init
* @param[in]  pos
* @param[in]  nNumber
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       初期化
* @attention  nNumber 0...左回転 1...右回転 
******************************************************************************/
void CRotIcon::Init(XMFLOAT2 pos,int nNumber)
{
	//Polygon初期化
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;
	m_nNumber = nNumber;

	//カラーを使用するか
	UseColor();

	//Left or Right
	if(nNumber == 0)
		SetTexture("data/Texture/UI/UI_ArrowLeft.png"); //左回転アイコン
	if (nNumber == 1)
		SetTexture("data/Texture/UI/UI_ArrowRight.png"); //右回転アイコン

	//設定
	SetPos(pos.x, pos.y);
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
void CRotIcon::Uninit()
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
void CRotIcon::Update()
{
	//ゲームが始まっていないなら更新しない
	if (!GLOBALDATA->GetStartFlag())
		return;

	//マウスポジション取得
	POINT* pos = CInput::GetMousePosition();

	//回転機能
	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y))) //マウスが重なっているとき
	{
		//赤くする
		SetColor(1.0f, 0.0f, 0.0f);

		//左クリック
		if (CInput::GetMouseButton(MOUSEBUTTON_L))
		{
			m_bActive = true;
			if (m_pPlayer)
			{
				//回転
				if (m_nNumber == 0)
					m_pPlayer->GetRotation().y += ROT_VALUE;
				if (m_nNumber == 1)
					m_pPlayer->GetRotation().y -= ROT_VALUE;
			}
		}
		else
			m_bActive = false;
	}
	else
	{
		m_bActive = false;
		SetColor(1.0f, 1.0f, 1.0f);
	}

	//[デバッグ] キー操作で回転
	if(CInput::GetKeyPress(VK_A) && !m_bActive && m_nNumber == 1)
		m_pPlayer->GetRotation().y -= ROT_VALUE;
	else if(CInput::GetKeyPress(VK_D) && !m_bActive && m_nNumber == 0)
		m_pPlayer->GetRotation().y += ROT_VALUE;


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
void CRotIcon::Draw()
{
	UI::Draw();

}