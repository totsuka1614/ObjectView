//=============================================================================
//
// ���b�V�� �N���X [Mesh.h]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#pragma once
#include "main.h"
#include "ConstantBuffer.h"

class CMesh
{
public:
	CMesh();
	~CMesh();

	HRESULT Init();

private:
	XMFLOAT3 m_vPos;
	XMFLOAT3 m_vDegree;
	XMFLOAT3 m_vScale;
	XMMATRIX m_mtxWorld;
	PSShaderType m_PStype;
	ID3D11Buffer* m_VertexBuffers;			//���_�o�b�t�@
	ID3D11Buffer* m_IndexBuffers;			//�C���f�b�N�X�o�b�t�@
	ConstantBuffer* m_pConstantBuffer[2];					//�萔�o�b�t�@ 0:���_ 1:�s�N�Z��
	MATERIAL m_Material;
};