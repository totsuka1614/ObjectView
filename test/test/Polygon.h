//=============================================================================
//
// 2Dポリゴン処理クラス定義 [Polygon.h]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#pragma once
#include "main.h"
#include "ConstantBuffer.h"
#include "BackBuffer.h"

class CPolygon
{
private:
	ID3D11ShaderResourceView* m_pTexture;	// テクスチャへのポインタ

	VERTEX_2D m_vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	XMFLOAT3 m_vPos;							// ポリゴンの移動量
	XMFLOAT3 m_vAngle;						// ポリゴンの回転量
	XMFLOAT3 m_vScale;						// ポリゴンのサイズ
	XMFLOAT4 m_vColor;						// ポリゴンの頂点カラー
	bool m_bInvalidate;						// 頂点データ更新フラグ

	XMFLOAT2 m_vPosTexFrame;					// UV座標
	XMFLOAT2 m_vSizeTexFrame;				// テクスチャ抽出サイズ

	ConstantBuffer* m_pConstantBuffer;			// 定数バッファ
	ID3D11Buffer* m_pVertexBuffer;			// 頂点バッファ
	ID3D11InputLayout* m_pInputLayout;		// 頂点フォーマット

	XMFLOAT4X4 m_mProj;						// 射影変換行列
	XMFLOAT4X4 m_mView;						// ビュー変換行列
	XMFLOAT4X4 m_mWorld;						// ワールド変換行列
	XMFLOAT4X4 m_mTex;						// テクスチャ変換行列

private:
	HRESULT MakeVertex(ID3D11Device* pDevice);
	void SetVertex(void);

public:
	virtual HRESULT Init();
	virtual void Fin();
	virtual void Draw();
	void SetTexture(ID3D11ShaderResourceView* pTexture);
	void SetPos(float fX, float fY);
	void SetSize(float fScaleX, float fScaleY);
	void SetAngle(float fAngle);
	void SetUV(float fU, float fV);
	void SetFrameSize(float fWidth, float fHeight);
	void SetColor(float fRed, float fGreen, float fBlue);
	void SetColor(XMFLOAT3 vColor);
	void SetAlpha(float fAlpha);
	void SetColor(float fRed, float fGreen, float fBlue, float fAlpha);
	void SetColor(XMFLOAT4 vColor);
};
