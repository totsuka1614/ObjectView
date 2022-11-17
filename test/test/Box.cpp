//=============================================================================
//
// 境界ボックス クラス [Box.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================

#include "Box.h"
#include <vector>
#include "BackBuffer.h"
#include "GUI.h"

void Box::Init(XMFLOAT3 vSize)
{	
	/*m_vMove.pos.x = m_vPos.x - m_vTarget->pos.x;
	m_vMove.pos.y = m_vPos.y - m_vTarget->pos.y;
	m_vMove.pos.z = m_vPos.z - m_vTarget->pos.z;

	m_vMove.scale.x = m_vScale.x - m_vTarget->scale.x;
	m_vMove.scale.y = m_vScale.y - m_vTarget->scale.y;
	m_vMove.scale.z = m_vScale.z - m_vTarget->scale.z;

	m_vMove.deglee.x = m_vDegree.x - m_vTarget->deglee.x;
	m_vMove.deglee.y = m_vDegree.y - m_vTarget->deglee.y;
	m_vMove.deglee.z = m_vDegree.z - m_vTarget->deglee.z;*/
									  
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

	int face[] = {
		4, 0, 5, 1,
		5, 1, 7, 3,
		7, 3, 6, 2,
		6, 2, 4, 0,
		0, 2, 1, 3,
		6, 4, 7, 5,
	};

	float nor[] = {
		 0.0f,  0.0f, -1.0f,
		 1.0f,  0.0f,  0.0f,
		 0.0f,  0.0f,  1.0f,
		-1.0f,  0.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
	};
	float uv[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};

	std::vector<VERTEX_3D> pVertexWk;
	int nIdx = 0;
	int* pIndexWk = new int[36];
	VERTEX_3D Vtx;
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 4; ++j) 
		{
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

	CMesh::Init(pVertexWk.data(), 24, pIndexWk, 36);
	//delete[] pIndexWk;
	//delete[] pVertexWk;

	GUI::Get()->Entry(*this);

}

void Box::Update()
{

	m_vMove.pos->x = m_vTarget->pos->x + m_vPos.x;
	m_vMove.pos->y = m_vTarget->pos->y + m_vPos.y;
	m_vMove.pos->z = m_vTarget->pos->z + m_vPos.z;
	
	m_vMove.scale->x = m_vTarget->scale->x + m_vScale.x;
	m_vMove.scale->y = m_vTarget->scale->y + m_vScale.y;
	m_vMove.scale->z = m_vTarget->scale->z + m_vScale.z;
	
	m_vMove.deglee->x = m_vTarget->deglee->x + m_vDegree.x;
	m_vMove.deglee->y = m_vTarget->deglee->y + m_vDegree.y;
	m_vMove.deglee->z = m_vTarget->deglee->z + m_vDegree.z;

	// ワールドマトリクス設定
	XMMATRIX translate = XMMatrixTranslation(m_vMove.pos->x, m_vMove.pos->y, m_vMove.pos->z);
	XMMATRIX rotate_x = XMMatrixRotationX(XMConvertToRadians(m_vMove.deglee->x));
	XMMATRIX rotate_y = XMMatrixRotationY(XMConvertToRadians(m_vMove.deglee->y));
	XMMATRIX rotate_z = XMMatrixRotationZ(XMConvertToRadians(m_vMove.deglee->z));
	XMMATRIX scale_mat = XMMatrixScaling(m_vMove.scale->x, m_vMove.scale->y, m_vMove.scale->z);
	m_mtxWorld = scale_mat * rotate_x * rotate_y * rotate_z * translate;
}

void Box::Draw()
{
	if (!GetEnable())
		return;

	if (m_bActive)
	{
		//m_Material.Diffuse = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);;
	}
	else
	{
		//m_Material.Diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	}

	CMesh::Draw(m_mtxWorld,m_VStype, m_PStype);
}

void Box::ColliderDraw()
{
	if (!GetEnable())
		return;

	BackBuffer::GetBuffer()->SetBlendState(BS_ALPHABLEND);

	if (m_bActive)
	{
		m_Material.Diffuse = XMFLOAT4(1.0f, 0.5f, 0.5f, 0.7f);;
		CMesh::Draw(m_mtxWorld);	
	}
	else
	{
		m_Material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.7f);
		CMesh::Draw(m_mtxWorld);
	}

	BackBuffer::GetBuffer()->SetBlendState();

}