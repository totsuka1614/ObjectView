/******************************************************************************
* 
* @file      Player.cpp
* @brief     プレイヤークラス
* @author    Totsuka Kensuke
* @date      2023/04/18
* @note      
* @attention 
* 
******************************************************************************/
#include "player.h"
#include "GUI.h"
#include <string.h>
#include <string>
#include "Input.h"
#include "CollisionList.h"
#include "GlobalData.h"
#include "SceneManager.h"

//定数・マクロ定義
#define MODEL_NAME "data/model/Jeep_Renegade_2016.fbx"
#define NORMALMAP_NAME "data/Texture/Player/NormalMap.png"
#define PLAYER_SPEED (0.05f)
#define GRAVITY_POWER (0.23f)

/******************************************************************************
* 
* @brief      Init
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       初期化
* @attention  
* 
******************************************************************************/
void CPlayer::Init(void)
{
	//モデルロード
	CFBXFile::Load(MODEL_NAME);
	//法線マップ生成
	CreateTextureFromFile(BACKBUFFER->GetDevice(), NORMALMAP_NAME, &m_pNormal);

	//初期値設定(せーぶでーたロード)
	m_Box = new CBox;
	m_Box->SetName("PlayerBoxCollider");
	m_Box->Init(XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_Box->LoadFile();
	m_vPos = XMFLOAT3(0.0f, 10.0f, 0.0f);
	m_vDegree = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_bJump = false;
	m_bStop = false;
	CObjectBase::LoadFile();

	//デバッグ表示登録
	GUI::Get()->Entry(*this);

	//コリジョン設定
	TARGET_TRANSFORM* target = new TARGET_TRANSFORM;
	m_bCol = true;
	m_bStart = false;
	target->pos = &m_vPos;
	target->scale = &m_vScale;
	target->deglee = &m_vDegree;
	m_Box->SetTarget(*target);

	//カメラ追従セット
	CCamera::Get()->SetPlayer(*this);
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       終了処理
* @attention  
******************************************************************************/
void CPlayer::Uninit(void)
{
	//セーブ
	m_Box->SaveFile();
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       更新処理
* @attention  
******************************************************************************/
void CPlayer::Update(void)
{
	//更新
	CObjectBase::Update();
	CModel::Update();
	m_Box->Update();

	//ゲーム終了時、スタート位置へ
	if (!GLOBALDATA->GetStartFlag() && m_bStart)
	{
		m_bStart = false;
		m_vPos = m_vStartPos;
		m_vDegree = m_vStartRot;
	}

	//ゲームが始まっていなければReturn
	if (!GLOBALDATA->GetStartFlag())
		return;
	

	//ゲームスタート時、位置を保存
	if (GLOBALDATA->GetStartFlag() && !m_bStart)
	{
		m_bStart = true;
		m_vStartPos = m_vPos;
		m_vStartRot = m_vDegree;
	}

	//ベロシティをゼロに
	m_vMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//重力
	m_vVel.y -= GRAVITY_POWER;

	//常に前進
	XMFLOAT4X4 mW; XMStoreFloat4x4(&mW, m_mtxWorld);
	XMFLOAT3 vZ = XMFLOAT3(mW._31, mW._32, mW._33);
	if (!m_bStop)
	{
		//前進
		m_vMove.x += vZ.x * PLAYER_SPEED;
		m_vMove.y += vZ.y * PLAYER_SPEED;
		m_vMove.z += vZ.z * PLAYER_SPEED;
	}

	//地面に当たっている場合
	if (COLLISION_AABB_TAG(m_Box,LAND))
	{
		//Ｙ値をゼロに
		m_vMove.y = 0.0f;
		m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_bJump = false;
	}
	//壁に当たっている場合
	if (COLLISION_AABB_TAG(m_Box, WALL))
	{
		//停止
		m_vMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vMove.x -= vZ.x * PLAYER_SPEED * 5.0f;
		m_vMove.y -= vZ.y * PLAYER_SPEED * 5.0f;
		m_vMove.z -= vZ.z * PLAYER_SPEED * 5.0f;
	}
	//ゴールに当たっている場合
	if (COLLISION_AABB_TAG(m_Box, GOAL))
	{
		//ゲームクリア
		//GLOBALDATA->Clear();
		MessageBox(NULL, _T("クリア"), _T(""), MB_OK);
		GLOBALDATA->Change();
	}

	//動いた量
	m_vMove.x += m_vVel.x;
	m_vMove.y += m_vVel.y;
	m_vMove.z += m_vVel.z;

	//最終的に動いた量を位置へ反映
	m_vPos.x += m_vMove.x;
	m_vPos.y += m_vMove.y;
	m_vPos.z += m_vMove.z;

}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       描画
* @attention  
******************************************************************************/
void CPlayer::Draw(void)
{
	//表示フラグ
	if (!GetEnable())
		return;
	//更新
	m_Box->Update();

	//Edge描画
	//if (m_bActive && !GLOBALDATA->GetStartFlag())
	//{
	//		BACKBUFFER->SetCullMode(CULLMODE_CW);
	//		FBXFile::EdgeDraw();
	//}

	//描画
	BACKBUFFER->SetCullMode(CULLMODE_CCW);	//カリング設定
	CFBXFile::Draw();
	BACKBUFFER->SetCullMode(CULLMODE_NONE);

	//コライダー描画
	m_Box->ColliderDraw();
}

/******************************************************************************
* 
* @brief      Jump
* @param[in]  power
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       ジャンプ
* @attention  引数にpowerをimpulse的に放出
******************************************************************************/
void CPlayer::Jump(float power)
{
	//ジャンプフラグ
	if (!m_bJump)
	{
		m_vVel.y = power;
		m_vMove.y += m_vVel.y;
		m_vPos.y += m_vMove.y;
		m_bJump = true;
	}
}