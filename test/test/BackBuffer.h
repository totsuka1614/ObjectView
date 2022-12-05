//=============================================================================
//
// バックバッファ クラス定義 [BackBuffer.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef BUFFER_H
#define BUFFER_H

#include "main.h"
#include "vertex.h"
#include "pixel.h"

enum TEXTURE_MAP
{
	BUMP_MAP,
	DISSOLVE_MAP,
	LAMP_MAP,

	MAX_MAP
};

class BackBuffer
{
public:
	BackBuffer() :
		m_pDevice(nullptr),
		m_pDeviceContext(nullptr),
		m_pSwapChain(nullptr),
		m_pRenderTargetView(nullptr),
		m_pDepthStencilTexture(nullptr),
		m_pDepthStencilView(nullptr)
	{
		m_PixelShader[0] = nullptr;
		m_PixelShader[1] = nullptr;
		m_VertexShader[0] = nullptr;
		m_VertexShader[1] = nullptr;
	}
	~BackBuffer() {};

	HRESULT Init(void);

	void Release(void);

	void StartRendering(void);
	void FinishRendering(void);
	void SetUpContext(VSShaderType = VERTEX,PSShaderType = PIXEL, D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void SetTexture(ID3D11ShaderResourceView*,int nNumber = 0);
	void SetBlendState(int nBlend = BS_NONE);
	void SetCullMode(int nCull = CULLMODE_NONE);
	void SetZBuffer(bool bEnable = true);
	ID3D11ShaderResourceView* GetTexture( TEXTURE_MAP nNumber) { return m_pTexture[nNumber]; }

	ID3D11Device*			GetDevice()	{return m_pDevice;}
	ID3D11DeviceContext*	GetDeviceContext() { return m_pDeviceContext; }
	Vertex*					GetVertexShader(VSShaderType type = VERTEX){return m_VertexShader[type];}
	ID3D11RenderTargetView*	GetRenderTargetView() {	return m_pRenderTargetView;	}
	ID3D11DepthStencilView* GetDepthStencilView() { return m_pDepthStencilView; }

	static BackBuffer* GetBuffer() { return m_pBuffer; }

private:
	
	HRESULT CreateDeviceAndSwapChain(void);
	HRESULT CreateRenderTargetView(void);
	HRESULT CreateDepthAndStencilView(void);
	HRESULT CreateShader(void);
	HRESULT CreateTextureSampler(void);
	HRESULT CreateBlendState(void);
	HRESULT CreateRasterizerState(void);
	HRESULT CreateDepthStencilState(void);
	void SetUpViewPort(void);
private:

	ID3D11Device*				m_pDevice;							//
	ID3D11DeviceContext*		m_pDeviceContext;
	IDXGISwapChain*				m_pSwapChain;			// スワップチェーン
	ID3D11RenderTargetView*		m_pRenderTargetView;	// フレームバッファ
	ID3D11Texture2D*			m_pDepthStencilTexture;	// Zバッファ用メモリ
	ID3D11DepthStencilView*		m_pDepthStencilView;	// Zバッファ
	ID3D11DepthStencilState*	m_pDSS[2];				// Z/ステンシル ステート
	ID3D11SamplerState*			m_pSamplerState;		//Textureサンプラー
	ID3D11BlendState*			m_pBlendState[MAX_BLENDSTATE];// ブレンド ステート
	ID3D11RasterizerState*		m_pRs[MAX_CULLMODE];	// ラスタライザ ステート

	ID3D11ShaderResourceView* m_pTexture[MAX_MAP];

	//パイプラインに登録するシェーダ
	Vertex* m_VertexShader[MAX_VSSHADER];
	Pixel* m_PixelShader[MAX_PSSHADER];

	static BackBuffer* m_pBuffer;
};

#endif // !BUFFER_H
