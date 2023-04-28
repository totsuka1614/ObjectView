/******************************************************************************
* 
* @file      Particle.cpp
* @brief     パーティクルクラス
* @author    Totsuka Kensuke
* @date      2023/04/18
* @note      マウスに追従する
* @attention 
* 
******************************************************************************/

// インクルードファイル
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "Particle.h"
#include "window.h"
#include "Input.h"

/******************************************************************************
* 
* @brief      Init
* @param[in]  nSize
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       初期化
* @attention  引数で数を設定
* 
******************************************************************************/
void CParticle::Init(int nSize)
{
	//パーティクル数設定
	m_Particle.resize(nSize);

	//初期化
	for (std::vector<PARTICLE>::iterator part = m_Particle.begin(); part != m_Particle.end(); part++)
	{
		part->status = 0;	// ステータスを初期化
		part->life = 0;	// ステータスを初期化
		part->size = XMFLOAT2(9.0f, 9.0f);	// ステータスを初期化
	}
	SetPos(0.0f, 0.0f);
	SetSize(9.0f, 9.0f);
	SetColor(1.0f, 0.0f, 0.0f);
	m_nInterval = 0;
	m_nSense = 3;
	//ポリゴン初期化
	CPolygon::Init();

	//パーティクルテクスチャ設定
	SetTexture("data/Texture/Particle001.png");
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       更新処理
* @attention  
* 
******************************************************************************/
void CParticle::Update()
{
	//パーティクル生成間隔
	if (m_nInterval < m_nSense)
		m_nInterval++;
	else
		m_nInterval = 0;

	//生成フラグ
	bool	bBorn;
	bBorn = false;

	//マウス座標取得
	POINT* mouse = CInput::GetMousePosition();

	mouse->x -= SCREEN_CENTER_X;
	mouse->y *= -1;
	mouse->y += SCREEN_CENTER_Y;

	//パーティクル生成
	for (std::vector<PARTICLE>::iterator part = m_Particle.begin(); part != m_Particle.end(); part++) {
		switch (part->status) {
		case 0:					// 待機状態
			if (!bBorn && m_nInterval >= m_nSense) {
				bBorn = true;
				part->status = 1;
			}
			break;
		case 1:
			//ランダム生成
			part->px = (float)mouse->x;
			part->py = (float)mouse->y;
			part->vx = (float)(rand() % 6 - 2.5f);
			part->vy = (float)(rand() % 7 - 2.5f);
			part->ax = 0.0f;
			part->ay = 0.0f;
			part->status = 2;
			// THRU
		case 2:
			//ランダムに動く
			part->vx += part->ax;
			part->vy += part->ay;

			part->px += part->vx;
			part->py += part->vy;
			
			part->life++;

			if (part->life >= 40) {
				part->size.x -= 0.5f;
				part->size.y -= 0.5f;
				if (part->size.x <= 0.0f)
				{
					part->status = 0;
					part->life = 0;
					part->size.x = 9.0f;
					part->size.y = 9.0f;
				}
			}
			break;
		}
	}
}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       描画処理
* @attention  
* 
******************************************************************************/
void CParticle::Draw()
{
	//アルファブレンド設定
	BACKBUFFER->SetBlendState(BS_ALPHABLEND);

	//描画
	for (std::vector<PARTICLE>::iterator part = m_Particle.begin(); part != m_Particle.end(); part++) {
		if (part->status == 2) {
			m_vPos.x = part->px;
			m_vPos.y = part->py;
			m_vScale.x = part->size.x;
			m_vScale.y = part->size.y;
			CPolygon::Draw();
		}
	}
}