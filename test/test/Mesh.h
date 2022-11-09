//=============================================================================
//
// メッシュ クラス [Mesh.h]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#pragma once
#include "main.h"
#include "ConstantBuffer.h"

class CMesh
{
public:
	CMesh() :m_vPos(0.0f, 0.0f, 0.0f),m_vScale(100.0f,100.0f,100.0f),m_vDegree(0.0f,0.0f,0.0f)
	{
		m_mtxWorld = XMMatrixIdentity();
		m_pConstantBuffer[0] = new ConstantBuffer;
		m_pConstantBuffer[1] = new ConstantBuffer;
	};
	~CMesh();

	HRESULT Init(VERTEX_3D Vertex[],int nVertex,int* Index,int nIndex);
	
	virtual void Update();
	virtual void Draw();

	XMFLOAT3& GetTransform(void) { return m_vPos; }
	XMFLOAT3& GetRotation(void) { return m_vDegree; }
	XMFLOAT3& GetScale(void) { return m_vScale; }

protected:
	XMFLOAT3 m_vPos;
	XMFLOAT3 m_vDegree;
	XMFLOAT3 m_vScale;
	XMMATRIX m_mtxWorld;
	MATERIAL m_Material;
private:
	PSShaderType m_PStype;
	ID3D11Buffer* m_VertexBuffers;			//頂点バッファ
	ID3D11Buffer* m_IndexBuffers;			//インデックスバッファ
	ID3D11InputLayout* m_InputLayout;		//入力レイアウト
	ConstantBuffer* m_pConstantBuffer[2];	//定数バッファ 0:頂点 1:ピクセル

	int m_nIndex;
};