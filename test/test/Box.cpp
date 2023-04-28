/******************************************************************************
* 
* @file      Box.cpp
* @brief     �{�b�N�X���b�V���N���X
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "Box.h"
#include <vector>
#include "BackBuffer.h"
#include "GUI.h"
#include "GlobalData.h"

/******************************************************************************
* 
* @brief      Init
* @param[in]  vSize
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ������
* @attention  
******************************************************************************/
void CBox::Init(XMFLOAT3 vSize)
{	
	//���_�ݒ�
	float vtx[] = {
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
	};

	//�ʂ̐ݒ�
	int face[] = {
		4, 0, 5, 1,
		5, 1, 7, 3,
		7, 3, 6, 2,
		6, 2, 4, 0,
		0, 2, 1, 3,
		6, 4, 7, 5,
	};

	//�@���̐ݒ�
	float nor[] = {
		 0.0f,  0.0f, -1.0f,
		 1.0f,  0.0f,  0.0f,
		 0.0f,  0.0f,  1.0f,
		-1.0f,  0.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
	};

	//UV�ݒ�
	float uv[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};

	//���b�V�����i�[��
	std::vector<VERTEX_3D> pVertexWk;
	
	//�v�Z�p
	int nIdx = 0;
	int* pIndexWk = new int[36];
	VERTEX_3D Vtx;
	
	for (int i = 0; i < 6; ++i)		//�ʂ̐�
	{
		for (int j = 0; j < 4; ++j)		//�ʂ̒��_��
		{
			//�����悭�킩���ĂȂ��v�Z
			Vtx.vtx.x = vtx[face[i * 4 + j] * 3 + 0];
			Vtx.vtx.y = vtx[face[i * 4 + j] * 3 + 1];
			Vtx.vtx.z = vtx[face[i * 4 + j] * 3 + 2];
			Vtx.nor.x = nor[i * 3 + 0];
			Vtx.nor.y = nor[i * 3 + 1];
			Vtx.nor.z = nor[i * 3 + 2];
			Vtx.tex.x = uv[j * 2 + 0];
			Vtx.tex.y = uv[j * 2 + 1];
			pVertexWk.push_back(Vtx);
			pIndexWk[i * 6 + j] = nIdx;
			++nIdx;
		}
		pIndexWk[i * 6 + 4] = pIndexWk[i * 6 + 2];
		pIndexWk[i * 6 + 5] = pIndexWk[i * 6 + 1];
	}

	//�i�[�����f�[�^���g��������
	CMesh::Init(pVertexWk.data(), 24, pIndexWk, 36);

	//�f�o�b�O�\���ɓo�^
	GUI::Get()->Entry(*this);
	
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �X�V
* @attention  
******************************************************************************/
void CBox::Update()
{
	/*
	(Collider�̏ꍇ)�^�[�Q�b�g�{BOX�����킹�Č��݂���肷��
	(������Box�̏ꍇ)�^�[�Q�b�g���O�Ȃ̂Ō��݂�BOX�����̂܂ܓ���
	*/
	
	//�ʒu
	m_vMove.pos->x = m_vTarget->pos->x + m_vPos.x;
	m_vMove.pos->y = m_vTarget->pos->y + m_vPos.y;
	m_vMove.pos->z = m_vTarget->pos->z + m_vPos.z;
	
	//�X�P�[��
	m_vMove.scale->x = m_vTarget->scale->x + m_vScale.x;
	m_vMove.scale->y = m_vTarget->scale->y + m_vScale.y;
	m_vMove.scale->z = m_vTarget->scale->z + m_vScale.z;
	
	//�p�x
	m_vMove.deglee->x = m_vTarget->deglee->x + m_vDegree.x;
	m_vMove.deglee->y = m_vTarget->deglee->y + m_vDegree.y;
	m_vMove.deglee->z = m_vTarget->deglee->z + m_vDegree.z;

	// ���[���h�}�g���N�X�ݒ�
	XMMATRIX translate = XMMatrixTranslation(m_vMove.pos->x, m_vMove.pos->y, m_vMove.pos->z);
	XMMATRIX rotate_x = XMMatrixRotationX(XMConvertToRadians(m_vMove.deglee->x));
	XMMATRIX rotate_y = XMMatrixRotationY(XMConvertToRadians(m_vMove.deglee->y));
	XMMATRIX rotate_z = XMMatrixRotationZ(XMConvertToRadians(m_vMove.deglee->z));
	XMMATRIX scale_mat = XMMatrixScaling(m_vMove.scale->x, m_vMove.scale->y, m_vMove.scale->z);
	m_mtxWorld = scale_mat * rotate_x * rotate_y * rotate_z * translate;
}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �`��
* @attention  
******************************************************************************/
void CBox::Draw()
{
	//�\�����邩�ۂ�
	if (!GetEnable())
		return;

	//�`��
	CMesh::Draw(m_mtxWorld,m_VStype, m_PStype);
}

/******************************************************************************
* 
* @brief      ColliderDraw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �����蔻��\��
* @attention  
******************************************************************************/
void CBox::ColliderDraw()
{
	//�Q�[�����n�܂�ƕ`�悵�Ȃ�
	if (GLOBALDATA->GetStartFlag())
		return;

	//�\�����邩�ۂ�
	if (!GetEnable())
		return;

	//���u�����h
	BACKBUFFER->SetBlendState(BS_ALPHABLEND);

	//Active�̏ꍇ
	if (m_bActive)
	{
		//�\����Ԃ�����
		m_Material.Diffuse = XMFLOAT4(1.0f, 0.5f, 0.5f, 0.7f);;
		CMesh::Draw(m_mtxWorld);	
	}
	else
	{
		m_Material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.7f);
		CMesh::Draw(m_mtxWorld);
	}

	//�߂�
	BACKBUFFER->SetBlendState();

}