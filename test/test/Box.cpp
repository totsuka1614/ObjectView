//=============================================================================
//
// 境界ボックス クラス [Box.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================

#include "Box.h"
#include <vector>
#include "BackBuffer.h"

void Box::Init(XMFLOAT3 vSize)
{
	LoadFile();
	
	m_vMove = m_vPos;

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

}

void Box::Update()
{
	m_vPos.x = m_vTarget.x + m_vMove.x;
	m_vPos.y = m_vTarget.y + m_vMove.y;
	m_vPos.z = m_vTarget.z + m_vMove.z;

	CMesh::Update();
}

void Box::Draw()
{
	BackBuffer::GetBuffer()->SetBlendState(BS_ALPHABLEND);

	CMesh::Draw();

	BackBuffer::GetBuffer()->SetBlendState();

}