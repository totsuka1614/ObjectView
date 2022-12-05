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

#define MODEL_NAME "data/model/unitychan.fbx"
#define PLAYER_SPEED (1.0f)
void CPlayer::Init(void)
{
	Model::Init();

	FBXFile::Load(MODEL_NAME);

	strcpy(m_cFileName, MODEL_NAME);


	m_Box = new Box;
	m_Box->SetName("PlayerBoxCollider");
	m_Box->Init(XMFLOAT3(100.0f, 100.0f, 100.0f));
	m_Box->LoadFile();
	m_vPos = XMFLOAT3(0.0f, 10.0f, 0.0f);
	m_vDegree = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_bJump = false;
	ObjectBase::LoadFile();

	TARGET_TRANSFORM* target = new TARGET_TRANSFORM;
	m_bCol = true;
	target->pos = &m_vPos;
	target->scale = &m_vScale;
	target->deglee = &m_vDegree;
	m_Box->SetTarget(*target);
}

void CPlayer::Uninit(void)
{
	Model::Uninit();
	m_Box->SaveFile();
}

void CPlayer::Update(void)
{
	Model::Update();

	m_Box->Update();

	if (!GlobalData::Get()->GetStartFlag())
		return;

	m_vMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vVel.y -= 0.3f;


	//常に前進
	XMFLOAT4X4 mW; XMStoreFloat4x4(&mW, m_mtxWorld);
	XMFLOAT3 vZ = XMFLOAT3(mW._31, mW._32, mW._33);
	m_vMove.x += vZ.x * PLAYER_SPEED;
	m_vMove.y += vZ.y * PLAYER_SPEED;
	m_vMove.z += vZ.z * PLAYER_SPEED;

	if (COLLISION_AABB(m_Box))
	{
		m_vMove.y = 0.0f;
		m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_bJump = false;
	}
	else
	{
	}
	
	if (COLLISION_AABB_TAG(m_Box, "Goal"))
	{
		m_vMove.y = 0.0f;
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

	if (m_bActive)
	{
		BackBuffer::GetBuffer()->SetCullMode(CULLMODE_CW);
		FBXFile::EdgeDraw();
	}

	BackBuffer::GetBuffer()->SetCullMode(CULLMODE_CCW);
	FBXFile::Draw();

	BackBuffer::GetBuffer()->SetCullMode(CULLMODE_NONE);



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