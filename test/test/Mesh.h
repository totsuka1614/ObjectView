//=============================================================================
//
// メッシュ クラス [Mesh.h]
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
	CMesh() :m_vPos(0.0f, 0.0f, 0.0f),m_vScale(100.0f,100.0f,100.0f),m_vDegree(0.0f,0.0f,0.0f)
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
	virtual void Draw();

	XMFLOAT3& GetTransform(void) { return m_vPos; }
	XMFLOAT3& GetRotation(void) { return m_vDegree; }
	XMFLOAT3& GetScale(void) { return m_vScale; }
	const char* GetName(void) { return m_cFileName; }

	void SetName(const char* name) { strcpy(m_cFileName, name); }

protected:
	
	void LoadFile();

	XMFLOAT3 m_vPos;
	XMFLOAT3 m_vDegree;
	XMFLOAT3 m_vScale;
	XMMATRIX m_mtxWorld;
	MATERIAL m_Material;
private:


	ID3D11Buffer* m_VertexBuffers;			//頂点バッファ
	ID3D11Buffer* m_IndexBuffers;			//インデックスバッファ
	ID3D11InputLayout* m_InputLayout;		//入力レイアウト
	ConstantBuffer* m_pConstantBuffer[2];	//定数バッファ 0:頂点 1:ピクセル

	char m_cFileName[256];
	int m_nIndex;
};