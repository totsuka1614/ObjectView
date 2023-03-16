/******************************************************************************
* 
* @file      JumpIcon.cpp
* @brief     ジャンプアイコンクラス
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "jumpicon.h"
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
void CJumpIcon::Init(XMFLOAT2 pos)
{
	//Polygon初期化
	CPolygon::Init();

	m_bEvent = false;
	m_bActive = false;

	//カラーを使用する
	UseColor();

	//設定
	SetTexture("data/Texture/UI/JumpIcon.png");
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
void CJumpIcon::Uninit()
{
	//Polygon終了
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
void CJumpIcon::Update()
{
	//ゲームが始まっていないなら更新しない
	if (!GLOBALDATA->GetStartFlag())
		return;

	//マウスポジション取得
	POINT* pos = CInput::GetMousePosition();

	//ジャンプ機能
	if (CollisionToPoint(XMFLOAT2(pos->x - SCREEN_CENTER_X, pos->y * -1.0f + SCREEN_CENTER_Y)))	//カーソルが重なっているとき
	{
		//Activeにし、カラーを赤くする
		m_bActive = true;
		SetColor(1.0f, 0.0f, 0.0f);
		
		//左クリック
		if (CInput::GetMouseTrigger(MOUSEBUTTON_L))
		{
			if (m_pPlayer)
			{
				//プレイヤージャンプ
				m_pPlayer->Jump(5.0f);
			}
		}
	}
	else
	{
		//NoActiveにし、色を戻す
		m_bActive = false;
		SetColor(1.0f, 1.0f, 1.0f);
	}

	//[デバッグ]スペースボタンでジャンプ
	if(CInput::GetKeyTrigger(VK_SPACE))
		m_pPlayer->Jump(5.0f);


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
void CJumpIcon::Draw()
{
	UI::Draw();
}