/******************************************************************************
* 
* @file      Player.cpp
* @brief     �v���C���[�N���X
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

//�萔�E�}�N����`
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
* @note       ������
* @attention  
* 
******************************************************************************/
void CPlayer::Init(void)
{
	//���f�����[�h
	CFBXFile::Load(MODEL_NAME);
	//�@���}�b�v����
	CreateTextureFromFile(BACKBUFFER->GetDevice(), NORMALMAP_NAME, &m_pNormal);

	//�����l�ݒ�(���[�ԂŁ[�����[�h)
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

	//�f�o�b�O�\���o�^
	GUI::Get()->Entry(*this);

	//�R���W�����ݒ�
	TARGET_TRANSFORM* target = new TARGET_TRANSFORM;
	m_bCol = true;
	m_bStart = false;
	target->pos = &m_vPos;
	target->scale = &m_vScale;
	target->deglee = &m_vDegree;
	m_Box->SetTarget(*target);

	//�J�����Ǐ]�Z�b�g
	CCamera::Get()->SetPlayer(*this);
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       �I������
* @attention  
******************************************************************************/
void CPlayer::Uninit(void)
{
	//�Z�[�u
	m_Box->SaveFile();
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       �X�V����
* @attention  
******************************************************************************/
void CPlayer::Update(void)
{
	//�X�V
	CObjectBase::Update();
	CModel::Update();
	m_Box->Update();

	//�Q�[���I�����A�X�^�[�g�ʒu��
	if (!GLOBALDATA->GetStartFlag() && m_bStart)
	{
		m_bStart = false;
		m_vPos = m_vStartPos;
		m_vDegree = m_vStartRot;
	}

	//�Q�[�����n�܂��Ă��Ȃ����Return
	if (!GLOBALDATA->GetStartFlag())
		return;
	

	//�Q�[���X�^�[�g���A�ʒu��ۑ�
	if (GLOBALDATA->GetStartFlag() && !m_bStart)
	{
		m_bStart = true;
		m_vStartPos = m_vPos;
		m_vStartRot = m_vDegree;
	}

	//�x���V�e�B���[����
	m_vMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//�d��
	m_vVel.y -= GRAVITY_POWER;

	//��ɑO�i
	XMFLOAT4X4 mW; XMStoreFloat4x4(&mW, m_mtxWorld);
	XMFLOAT3 vZ = XMFLOAT3(mW._31, mW._32, mW._33);
	if (!m_bStop)
	{
		//�O�i
		m_vMove.x += vZ.x * PLAYER_SPEED;
		m_vMove.y += vZ.y * PLAYER_SPEED;
		m_vMove.z += vZ.z * PLAYER_SPEED;
	}

	//�n�ʂɓ������Ă���ꍇ
	if (COLLISION_AABB_TAG(m_Box,LAND))
	{
		//�x�l���[����
		m_vMove.y = 0.0f;
		m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_bJump = false;
	}
	//�ǂɓ������Ă���ꍇ
	if (COLLISION_AABB_TAG(m_Box, WALL))
	{
		//��~
		m_vMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vMove.x -= vZ.x * PLAYER_SPEED * 5.0f;
		m_vMove.y -= vZ.y * PLAYER_SPEED * 5.0f;
		m_vMove.z -= vZ.z * PLAYER_SPEED * 5.0f;
	}
	//�S�[���ɓ������Ă���ꍇ
	if (COLLISION_AABB_TAG(m_Box, GOAL))
	{
		//�Q�[���N���A
		//GLOBALDATA->Clear();
		MessageBox(NULL, _T("�N���A"), _T(""), MB_OK);
		GLOBALDATA->Change();
	}

	//��������
	m_vMove.x += m_vVel.x;
	m_vMove.y += m_vVel.y;
	m_vMove.z += m_vVel.z;

	//�ŏI�I�ɓ������ʂ��ʒu�֔��f
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
* @note       �`��
* @attention  
******************************************************************************/
void CPlayer::Draw(void)
{
	//�\���t���O
	if (!GetEnable())
		return;
	//�X�V
	m_Box->Update();

	//Edge�`��
	//if (m_bActive && !GLOBALDATA->GetStartFlag())
	//{
	//		BACKBUFFER->SetCullMode(CULLMODE_CW);
	//		FBXFile::EdgeDraw();
	//}

	//�`��
	BACKBUFFER->SetCullMode(CULLMODE_CCW);	//�J�����O�ݒ�
	CFBXFile::Draw();
	BACKBUFFER->SetCullMode(CULLMODE_NONE);

	//�R���C�_�[�`��
	m_Box->ColliderDraw();
}

/******************************************************************************
* 
* @brief      Jump
* @param[in]  power
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       �W�����v
* @attention  ������power��impulse�I�ɕ��o
******************************************************************************/
void CPlayer::Jump(float power)
{
	//�W�����v�t���O
	if (!m_bJump)
	{
		m_vVel.y = power;
		m_vMove.y += m_vVel.y;
		m_vPos.y += m_vMove.y;
		m_bJump = true;
	}
}