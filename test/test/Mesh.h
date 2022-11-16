//=============================================================================
//
// ���b�V�� �N���X [Mesh.h]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#pragma once
#include "main.h"
#include "ConstantBuffer.h"
#include <string.h>
#include "ObjectBase.h"

class CMesh : public ObjectBase
{
public:
	CMesh()
	{
		m_mtxWorld = XMMatrixIdentity();
		m_pConstantBuffer[0] = new ConstantBuffer;
		m_pConstantBuffer[1] = new ConstantBuffer;
	};
	~CMesh()
	{
		SAFE_RELEASE(m_VertexBuffers);
		SAFE_RELEASE(m_IndexBuffers);
		SAFE_RELEASE(m_InputLayout);
		SAFE_DELETE(m_pConstantBuffer[0]);
		SAFE_DELETE(m_pConstantBuffer[1]);
	};

	HRESULT Init(VERTEX_3D Vertex[],int nVertex,int* Index,int nIndex);
	
	virtual void Update();
	virtual void Draw(XMMATRIX& mtxWorld, VSShaderType vstype = VERTEX, PSShaderType pstype = UNLIT);

protected:
	XMMATRIX m_mtxWorld;
	MATERIAL m_Material;
private:

	ID3D11Buffer* m_VertexBuffers;			//���_�o�b�t�@
	ID3D11Buffer* m_IndexBuffers;			//�C���f�b�N�X�o�b�t�@
	ID3D11InputLayout* m_InputLayout;		//���̓��C�A�E�g
	ConstantBuffer* m_pConstantBuffer[2];	//�萔�o�b�t�@ 0:���_ 1:�s�N�Z��
	
	int m_nIndex;
	
};