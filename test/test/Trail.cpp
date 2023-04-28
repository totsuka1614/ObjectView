/******************************************************************************
* 
* @file      Trail.cpp
* @brief     追尾パーティクル
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#include "Trail.h"
#include "Input.h"
#include "BackBuffer.h"

/******************************************************************************
* 
* @brief      Init
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/27
* @note       初期化
* @attention  
******************************************************************************/
void CTrail::Init()
{
	for (int i = 0; i < TRAIL_MAX; i++) {
		m_Trail[i].status = 0;	// ステータスを初期化
		m_Trail[i].life = 0;	// ステータスを初期化
		m_Trail[i].size = XMFLOAT2(9.0f, 9.0f);	// ステータスを初期化
	}

	//初期化
	CPolygon::Init();

	//初期値設定
	SetTexture("data/Texture/Particle002.png");
	SetPos(0.0f, 0.0f);
	SetSize(9.0f, 9.0f);
	SetColor(1.0f, 1.0f, 1.0f);
	m_nInterval = 0;
	m_OldMouse = *CInput::GetMousePosition();
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/27
* @note       更新処理
* @attention  
******************************************************************************/
void CTrail::Update() 
{
	//生成間隔
	m_nInterval++;	

	//マウス座標取得
	POINT* mouse = CInput::GetMousePosition();
	XMFLOAT2 pos = XMFLOAT2(mouse->x - m_OldMouse.x, mouse->y - m_OldMouse.y);

	//パーティクル更新
	for (int i = 0; i < TRAIL_MAX; i++) {
		switch (m_Trail[i].status) {
		case 0:					// 待機状態
			if (m_nInterval >= 1 && (pos.x != 0.0f || pos.y != 0.0f)) {
				m_Trail[i].status = 1;
				m_nInterval = 0;
			}
			break;
		case 1:
			m_Trail[i].px = (float)mouse->x;
			m_Trail[i].py = (float)mouse->y;
			m_Trail[i].vx = 0.0f;
			m_Trail[i].vy = 0.0f;
			m_Trail[i].ax = 0.0f;
			m_Trail[i].ay = 0.0f;
			m_Trail[i].status = 2;
			// THRU
		case 2:

			m_Trail[i].vx += m_Trail[i].ax;
			m_Trail[i].vy += m_Trail[i].ay;

			m_Trail[i].px += m_Trail[i].vx;
			m_Trail[i].py += m_Trail[i].vy;

			m_Trail[i].life++;

			if (m_Trail[i].life >= 10) {
				m_Trail[i].size.x -= 0.5f;
				m_Trail[i].size.y -= 0.5f;
				if (m_Trail[i].size.x <= 0.0f)
				{
					m_Trail[i].status = 0;
					m_Trail[i].life = 0;
					m_Trail[i].size.x = 9.0f;
					m_Trail[i].size.y = 9.0f;
				}
			}
			break;
		}
	}

	//マウス座標更新
	m_OldMouse = *CInput::GetMousePosition();
}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/27
* @note       描画
* @attention  
******************************************************************************/
void CTrail::Draw()
{
	//αブレンドセット
	BACKBUFFER->SetBlendState(BS_ALPHABLEND);
	//描画
	for (int i = 0; i < TRAIL_MAX; i++) {
		if (m_Trail[i].status == 2) {
			m_vPos.x = m_Trail[i].px;
			m_vPos.y = m_Trail[i].py;
			m_vScale.x = m_Trail[i].size.x;
			m_vScale.y = m_Trail[i].size.y;
			CPolygon::Draw();
		}
	}
}