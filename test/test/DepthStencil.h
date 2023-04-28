/******************************************************************************
* 
* @file      DepthStencil.h
* @brief     デプスステンシルクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//インクルード部
#include "main.h"

class CDepthStencil
{
public:
	//コンストラクタ
	CDepthStencil();
	//デストラクタ
	~CDepthStencil();

	UINT GetWidth() { return m_fWidth; }
	UINT GetHeight() { return m_fHeight; }
	ID3D11DepthStencilView* GetView() const { return m_pDSV; }
	ID3D11ShaderResourceView* GetResource() const { return m_pSRV; }
	HRESULT Create(DXGI_FORMAT format, float width = SCREEN_WIDTH, float height = SCREEN_HEIGHT, const void* pData = nullptr);

private:
	float m_fWidth;
	float m_fHeight;

	ID3D11ShaderResourceView *m_pSRV;

protected:
	ID3D11Texture2D* m_pTex;
	ID3D11DepthStencilView* m_pDSV;
};