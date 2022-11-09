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
	CMesh();
	~CMesh();

	HRESULT Init();

private:
	XMFLOAT3 m_vPos;
	XMFLOAT3 m_vDegree;
	XMFLOAT3 m_vScale;
	XMMATRIX m_mtxWorld;
	PSShaderType m_PStype;
	ID3D11Buffer* m_VertexBuffers;			//頂点バッファ
	ID3D11Buffer* m_IndexBuffers;			//インデックスバッファ
	ConstantBuffer* m_pConstantBuffer[2];					//定数バッファ 0:頂点 1:ピクセル
	MATERIAL m_Material;
};