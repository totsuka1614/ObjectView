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

class CMesh
{
public:
	CMesh() :m_vPos(0.0f, 0.0f, 0.0f),m_vScale(100.0f,100.0f,100.0f),m_vDegree(0.0f,0.0f,0.0f),m_bActive(false),m_bEnable(true),m_eType(BOX)
	{
		strcpy(m_cFileName, "Default");
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

	XMFLOAT3& GetTransform(void) { return m_vPos; }
	XMFLOAT3& GetRotation(void) { return m_vDegree; }
	XMFLOAT3& GetScale(void) { return m_vScale; }
	const char* GetName(void) { return m_cFileName; }
	bool& GetEnable(void) { return m_bEnable; }
	bool& GetActive(void) { return m_bActive; }

	void SetName(const char* name) { strcpy(m_cFileName, name); }
	void SaveFile();
	void LoadFile(SAVE_DATA save);

protected:
	

	XMFLOAT3 m_vPos;
	XMFLOAT3 m_vDegree;
	XMFLOAT3 m_vScale;
	XMMATRIX m_mtxWorld;
	MATERIAL m_Material;
	bool m_bActive;
private:

	ID3D11Buffer* m_VertexBuffers;			//���_�o�b�t�@
	ID3D11Buffer* m_IndexBuffers;			//�C���f�b�N�X�o�b�t�@
	ID3D11InputLayout* m_InputLayout;		//���̓��C�A�E�g
	ConstantBuffer* m_pConstantBuffer[2];	//�萔�o�b�t�@ 0:���_ 1:�s�N�Z��
	
	bool m_bEnable;
	char m_cFileName[256];
	int m_nIndex;
	ObjectType m_eType;
	
};