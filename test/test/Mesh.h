/******************************************************************************
* 
* @file      Mesh.h
* @brief     メッシュクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//インクルード部
#include "main.h"
#include "ConstantBuffer.h"
#include <string.h>
#include "ObjectBase.h"

class CMesh : public CObjectBase
{
public:
	//コンストラクタ
	CMesh()
	{
		m_mtxWorld = XMMatrixIdentity();
		m_pConstantBuffer[0] = new CConstantBuffer;
		m_pConstantBuffer[1] = new CConstantBuffer;
	};
	//デストラクタ
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
private:
	ID3D11Buffer* m_VertexBuffers;			//頂点バッファ
	ID3D11Buffer* m_IndexBuffers;			//インデックスバッファ
	ID3D11InputLayout* m_InputLayout;		//入力レイアウト
	CConstantBuffer* m_pConstantBuffer[2];	//定数バッファ 0:頂点 1:ピクセル
	
	int m_nIndex;
	
};