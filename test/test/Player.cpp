//=============================================================================
//
// Player クラス [Player.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "player.h"
#include "GUI.h"
#include <string.h>
#include <string>
#include "Input.h"
#include "CollisionList.h"
#include "GlobalData.h"
#include "SceneManager.h"

#define MODEL_NAME "data/model/Jeep_Renegade_2016.fbx"
#define PLAYER_SPEED (0.05f)

void CPlayer::Init(void)
{

	FBXFile::Load(MODEL_NAME);
	CreateTextureFromFile(BACKBUFFER->GetDevice(), "data/Texture/Player/NormalMap.png", &m_pNormal);

	m_Box = new Box;
	m_Box->SetName("PlayerBoxCollider");
	m_Box->Init(XMFLOAT3(100.0f, 100.0f, 100.0f));
	m_Box->LoadFile();
	m_vPos = XMFLOAT3(0.0f, 10.0f, 0.0f);
	m_vDegree = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_bJump = false;
	m_bStop = false;
	ObjectBase::LoadFile();

	GUI::Get()->Entry(*this);
	TARGET_TRANSFORM* target = new TARGET_TRANSFORM;
	m_bCol = true;
	m_bStart = false;
	target->pos = &m_vPos;
	target->scale = &m_vScale;
	target->deglee = &m_vDegree;
	m_Box->SetTarget(*target);

	CCamera::Get()->SetPlayer(*this);
}

void CPlayer::Uninit(void)
{
	m_Box->SaveFile();
}

void CPlayer::Update(void)
{
	ObjectBase::Update();

	Model::Update();

	m_Box->Update();


	if (!GLOBALDATA->GetStartFlag() && m_bStart)
	{
		m_bStart = false;
		m_vPos = m_vStartPos;
		m_vDegree = m_vStartRot;
	}

	if (!GLOBALDATA->GetStartFlag())
		return;

	if (GLOBALDATA->GetStartFlag() && !m_bStart)
	{
		m_bStart = true;
		m_vStartPos = m_vPos;
		m_vStartRot = m_vDegree;
	}

	
	m_vMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vVel.y -= 0.23f;


	//常に前進
	XMFLOAT4X4 mW; XMStoreFloat4x4(&mW, m_mtxWorld);
	XMFLOAT3 vZ = XMFLOAT3(mW._31, mW._32, mW._33);
	if (!m_bStop)
	{
		m_vMove.x += vZ.x * PLAYER_SPEED;
		m_vMove.y += vZ.y * PLAYER_SPEED;
		m_vMove.z += vZ.z * PLAYER_SPEED;
	}

	if (COLLISION_AABB_TAG(m_Box,LAND))
	{
		m_vMove.y = 0.0f;
		m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_bJump = false;
	}
	if (COLLISION_AABB_TAG(m_Box, WALL))
	{
		m_vMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vMove.x -= vZ.x * PLAYER_SPEED * 5.0f;
		m_vMove.y -= vZ.y * PLAYER_SPEED * 5.0f;
		m_vMove.z -= vZ.z * PLAYER_SPEED * 5.0f;
	}
	if (COLLISION_AABB_TAG(m_Box, GOAL))
	{
		//GLOBALDATA->Clear();
		MessageBox(NULL, _T("クリアぺこ"), _T("やったぺこ！"), MB_OK);
		GLOBALDATA->Change();
	}

	m_vMove.x += m_vVel.x;
	m_vMove.y += m_vVel.y;
	m_vMove.z += m_vVel.z;

	m_vPos.x += m_vMove.x;
	m_vPos.y += m_vMove.y;
	m_vPos.z += m_vMove.z;

}

void CPlayer::Draw(void)
{
	if (!GetEnable())
		return;

	m_Box->Update();

	if (m_bActive && !GLOBALDATA->GetStartFlag())
	{
		BACKBUFFER->SetCullMode(CULLMODE_CW);
		//FBXFile::EdgeDraw();
	}

	BACKBUFFER->SetCullMode(CULLMODE_CCW);
	FBXFile::Draw();

	BACKBUFFER->SetCullMode(CULLMODE_NONE);



	m_Box->ColliderDraw();
}

void CPlayer::Jump(float power)
{
	if (!m_bJump)
	{
		m_vVel.y = power;
		m_vMove.y += m_vVel.y;
		m_vPos.y += m_vMove.y;
		m_bJump = true;
	}
}