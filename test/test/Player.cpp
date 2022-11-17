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

#define MODEL_NAME "data/model/airplane.fbx"

void CPlayer::Init(void)
{
	Model::Init();

	m_ModelData->Load(MODEL_NAME);

	m_ModelData->SetMaterial(m_Material.Diffuse);

	strcpy(m_cFileName, MODEL_NAME);


	m_Box = new Box;
	m_Box->SetName("PlayerBoxCollider");
	m_Box->Init(XMFLOAT3(100.0f, 100.0f, 100.0f));
	m_Box->LoadFile();
	m_vPos = XMFLOAT3(0.0f, 10.0f, 0.0f);
	m_vDegree = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	ObjectBase::LoadFile();

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
	m_Box->SaveFile();
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

	if (m_bActive)
	{
		BackBuffer::GetBuffer()->SetCullMode(CULLMODE_CW);
		m_ModelData->Draw(m_Material,m_mtxWorld, EDGEVS, EDGEPS);
	}

	BackBuffer::GetBuffer()->SetCullMode(CULLMODE_CCW);
	m_ModelData->Draw(m_Material,m_mtxWorld, m_VStype, m_PStype);

	BackBuffer::GetBuffer()->SetCullMode(CULLMODE_NONE);

	m_Box->ColliderDraw();
}
