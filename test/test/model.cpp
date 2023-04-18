/******************************************************************************
* 
* @file      model.cpp
* @brief     ���f���N���X
* @author    Totsuka Kensuke
* @date      2023/04/18
* @note      
* @attention 
* 
******************************************************************************/
#include "model.h"
#include "GUI.h"
#include <string>
#include "Input.h"

/******************************************************************************
* 
* @brief      ~Model
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       �f�X�g���N�^
* @attention  
* 
******************************************************************************/
Model::~Model()
{
	//���
	IMGUI->GetObjectList().remove(m_Box);
}

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
void Model::Init(void)
{
	//���f�����[�h
	FBXFile::Load(m_cFileName);

	//�����l�ݒ�
	m_Box = new Box;
	char name[256];
	strcpy(name, m_cName);
	strcat(name, "BoxCollider");
	m_Box->SetName(name);
	m_Box->Init(XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_Box->LoadFile();

	//�O���t�@�C�����[�h
	ObjectBase::LoadFile();

	//�R���C�_�[�ݒ�(���f���ɒǏ])
	TARGET_TRANSFORM* target = new TARGET_TRANSFORM;
	m_bCol = true;
	target->pos = &m_vPos;
	target->scale = &m_vScale;
	target->deglee = &m_vDegree;
	m_Box->SetTarget(*target);
	GUI::Get()->Entry(*this);
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       �I������
* @attention  
* 
******************************************************************************/
void Model::Uninit(void)
{
	//�Z�[�u
	m_Box->SaveFile();
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       �X�V����
* @attention  
* 
******************************************************************************/
void Model::Update(void)
{
	// ���[���h�}�g���N�X�ݒ�
	XMMATRIX translate = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMMATRIX rotate_x = XMMatrixRotationX(XMConvertToRadians(m_vDegree.x));
	XMMATRIX rotate_y = XMMatrixRotationY(XMConvertToRadians(m_vDegree.y));
	XMMATRIX rotate_z = XMMatrixRotationZ(XMConvertToRadians(m_vDegree.z));
	XMMATRIX scale_mat =XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	m_mtxWorld = scale_mat * rotate_x * rotate_y * rotate_z * translate;	
}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       �`�揈��
* @attention  
* 
******************************************************************************/
void Model::Draw(void)
{
	//�`��
	FBXFile::Draw();
	m_Box->ColliderDraw();
}