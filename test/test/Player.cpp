//=============================================================================
//
// Player ƒNƒ‰ƒX [Player.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "player.h"
#include "GUI.h"
#include <string.h>
#include <string>
#include "Input.h"

#define MODEL_NAME "data/model/unitychan.fbx"

void CPlayer::Init(void)
{
	Model::Init();

	m_ModelData->Load(MODEL_NAME);

	strcpy(m_cFileName, MODEL_NAME);


	m_Box = new Box;
	m_Box->SetName("PlayerBoxCollider");
	m_Box->Init(XMFLOAT3(100.0f, 100.0f, 100.0f));

	m_vPos = XMFLOAT3(0.0f, 10.0f, 0.0f);
	m_vDegree = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	Model::LoadFile();

	TARGET_TRANSFORM* target = new TARGET_TRANSFORM;
	target->pos = &m_vPos;
	target->scale = &m_vScale;
	target->deglee = &m_vDegree;
	m_Box->SetTarget(*target);

	GUI::Get()->Entry(*this);

}

void CPlayer::Uninit(void)
{
	Model::Uninit();
}

void CPlayer::Update(void)
{
	Model::Update();

	m_Box->Update();

}

void CPlayer::Draw(void)
{
	if (!GetEnable())
		return;

	if (bActive)
	{
		BackBuffer::GetBuffer()->SetCullMode(CULLMODE_CW);
		m_ModelData->Draw(m_mtxWorld, EDGEVS, EDGEPS);
	}

	BackBuffer::GetBuffer()->SetCullMode(CULLMODE_CCW);
	m_ModelData->Draw(m_mtxWorld, m_VStype, m_PStype);

	BackBuffer::GetBuffer()->SetCullMode(CULLMODE_NONE);

	m_Box->ColliderDraw();
}
