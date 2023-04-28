/******************************************************************************
* 
* @file      BackBuffer.cpp
* @brief     バッファが色々ある
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "BackBuffer.h"
#include "window.h"
#include "Texture.h"

#pragma comment(lib, "d3d11")

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CBackBuffer g_backbuffer;	//インスタンス

CBackBuffer* CBackBuffer::m_pBuffer = &g_backbuffer;  // 現在のバッファ

/******************************************************************************
* 
* @brief      Init
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       初期化(成功でS_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::Init(void)
{
	m_pDevice = nullptr;
	m_pDeviceContext = nullptr; 
	m_pSwapChain = nullptr;
	m_pRenderTargetView = nullptr;
	m_pDepthStencilTexture = nullptr;
	m_pDepthStencilView = nullptr;

	HRESULT hr = S_OK;

	//スワップチェーン作成		//フロントバッファ、バックバッファ
	hr = CreateDeviceAndSwapChain();
	if (FAILED(hr))
		return hr;

	//レンダーターゲット作成	//描画するためのリソースの確保
	hr = CreateRenderTargetView();
	if (FAILED(hr))
		return hr;

	//Zバッファ/ステンシルバッファ作成	//物体の前後を知る
	hr = CreateDepthAndStencilView();
	if (FAILED(hr))
		return hr;

	//シェーダ作成
	hr = CreateShader();
	if (FAILED(hr))
		return hr;

	//ブレンドステート作成
	hr = CreateBlendState();
	if (FAILED(hr))
		return hr;

	//ビューポート設定
	SetUpViewPort();

	//ラスタライザ設定
	CreateRasterizerState();

	//深度バッファ作成
	CreateDepthStencilState();

	//テストテクスチャ作成
	CreateTextureFromFile(m_pDevice, "data/Texture/Mapping/NormalMap.png", &m_pTexture[BUMP_MAP]);
	CreateTextureFromFile(m_pDevice, "data/Texture/Mapping/dissolve.png", &m_pTexture[DISSOLVE_MAP]);
	CreateTextureFromFile(m_pDevice, "data/Texture/Mapping/Lamp.png", &m_pTexture[LAMP_MAP]);

	return hr;
}


/******************************************************************************
* 
* @brief      SetUpContext
* @param[in]  VStype
* @param[in]  PStype
* @param[in]  topology
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       描画のセットアップ
* @attention  引数なしの場合、基本の描画になる
******************************************************************************/
void CBackBuffer::SetUpContext(VSShaderType VStype,PSShaderType PStype , D3D_PRIMITIVE_TOPOLOGY topology)
{
	// プリミティブの形状を指定
	m_pDeviceContext->IASetPrimitiveTopology(topology);
	// VerteXShader、PixelShaderを設定
	m_pDeviceContext->VSSetShader(
		m_VertexShader[VStype]->GetShader(),	// 使用するVertexShder
		nullptr,
		0);									// ClassInstanceの数
	m_pDeviceContext->PSSetShader(
		m_PixelShader[PStype]->GetShader(),
		nullptr,
		0);
}

/******************************************************************************
* 
* @brief      SetTexture
* @param[in]  texture
* @param[in]  nNumber
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ピクセルシェーダで利用するテクスチャをセット
* @attention  nNumberはスロット番号
******************************************************************************/
void CBackBuffer::SetTexture(ID3D11ShaderResourceView* texture,int nNumber)
{
	// Samplerの設定
	m_pDeviceContext->PSSetSamplers(
		0,					// スロット番号
		1,					// サンプラーの数
		&m_pSamplerState);	// ID3D11SamplerState

	// PixelShaderで使用するテクスチャの設定
	m_pDeviceContext->PSSetShaderResources(
		nNumber,								// スロット番号
		1,								// リソースの数
		&texture);						// ID3D11ShaderResourceView
}

/******************************************************************************
* 
* @brief      SetBlendState
* @param[in]  nBlend
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ブレンドステートをセット
* @attention  EBlendStateを参照
******************************************************************************/
void CBackBuffer::SetBlendState(int nBlend)
{
	if (nBlend >= 0 && nBlend < MAX_BLENDSTATE) {
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_pDeviceContext->OMSetBlendState(m_pBlendState[nBlend], blendFactor, 0xffffffff);
	}
}

/******************************************************************************
* 
* @brief      SetCullMode
* @param[in]  nCull
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       カリングセット
* @attention  ECullModeを参照
******************************************************************************/
void CBackBuffer::SetCullMode(int nCull)
{
	if (nCull >= 0 && nCull < MAX_CULLMODE) {
		m_pDeviceContext->RSSetState(m_pRs[nCull]);
	}
}

/******************************************************************************
* 
* @brief      SetZBuffer
* @param[in]  bEnable
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       深度バッファ有効無効のセット
* @attention  
******************************************************************************/
void CBackBuffer::SetZBuffer(bool bEnable)
{
	m_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : m_pDSS[1], 0);
}

/******************************************************************************
* 
* @brief      CreateDeviceAndSwapChain
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       スワップチェーンの作成(成功でS_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateDeviceAndSwapChain(void)
{
	HRESULT hr = S_OK;

	// デバイス、スワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = CWindow::GetWindow()->GetWindowHandle();
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = true;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &scd,
		&m_pSwapChain, &m_pDevice, nullptr, &m_pDeviceContext);
	if (FAILED(hr)) {
		return hr;
	}

	return hr;
}

/******************************************************************************
* 
* @brief      CreateRenderTargetView
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       レンダ―ターゲット作成(成功でS_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateRenderTargetView(void)
{
	HRESULT hr = S_OK;

	// RenderTargetViewの対象となるBufferの取得
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pRenderTargetTexture);
	if (FAILED(hr))
		return hr;

	// BufferからRenderTargetViewの作成
	hr = m_pDevice->CreateRenderTargetView(m_pRenderTargetTexture, NULL, &m_pRenderTargetView);
	if (FAILED(hr))
		return hr;

	return hr;
}

/******************************************************************************
* 
* @brief      CreateDepthAndStencilView
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       Zバッファ＆ステンシル作成(成功でS_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateDepthAndStencilView()
{
	// Zバッファ用テクスチャ生成
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = SCREEN_WIDTH;
	td.Height = SCREEN_HEIGHT;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRESULT hr = m_pDevice->CreateTexture2D(&td, nullptr, &m_pDepthStencilTexture);
	if (FAILED(hr)) {
		return hr;
	}

	// Zバッファターゲットビュー生成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture,
		&dsvd, &m_pDepthStencilView);
	if (FAILED(hr)) {
		return hr;
	}

	// 各ターゲットビューをレンダーターゲットに設定
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	return hr;
}

/******************************************************************************
* 
* @brief      CreateShader
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       シェーダ作成(成功でS_OK)
* @attention  追加する度、書き足す
******************************************************************************/
HRESULT CBackBuffer::CreateShader(void)
{
	//3D
	m_VertexShader[VERTEX] = new CVertex;
	if (m_VertexShader[VERTEX]->Create(m_pDevice, "data/shader/vertex.cso") == false)
	{
		return false;
	}

	//アウトライン
	m_VertexShader[EDGEVS] = new CVertex;
	if (m_VertexShader[EDGEVS]->Create(m_pDevice, "data/shader/EdgeVS.cso") == false)
	{
		return false;
	}

	//2D
	m_VertexShader[VERTEX2D] = new CVertex;
	if (m_VertexShader[VERTEX2D]->Create(m_pDevice, "data/shader/Vertex2D.cso") == false)
	{
		return false;
	}

	//BUMP_MAP
	m_VertexShader[BUMPVS] = new CVertex;
	if (m_VertexShader[BUMPVS]->Create(m_pDevice, "data/shader/BumpVS.cso") == false)
	{
		return false;
	}

	//影
	m_VertexShader[SHADOWVS] = new CVertex;
	if (m_VertexShader[SHADOWVS]->Create(m_pDevice, "data/shader/ShadowVS.cso") == false)
	{
		return false;
	}

	//深度シャドウ
	m_VertexShader[DEPTHWRITEVS] = new CVertex;
	if (m_VertexShader[DEPTHWRITEVS]->Create(m_pDevice, "data/shader/DepthWriteVS.cso") == false)
	{
		return false;
	}

	//3D
	m_PixelShader[PIXEL] = new CPixel;
	if (m_PixelShader[PIXEL]->Create(m_pDevice, "data/shader/pixel.cso") == false)
	{
		return false;
	}

	//拡散光
	m_PixelShader[LAMBERT] = new CPixel;
	if (m_PixelShader[LAMBERT]->Create(m_pDevice, "data/shader/lambert.cso") == false)
	{
		return false;
	}

	//鏡面反射
	m_PixelShader[PHONG] = new CPixel;
	if (m_PixelShader[PHONG]->Create(m_pDevice, "data/shader/Phong.cso") == false)
	{
		return false;
	}

	//ライトなし
	m_PixelShader[UNLIT] = new CPixel;
	if (m_PixelShader[UNLIT]->Create(m_pDevice, "data/shader/Unlit.cso") == false)
	{
		return false;
	}

	//アウトライン
	m_PixelShader[EDGEPS] = new CPixel;
	if (m_PixelShader[EDGEPS]->Create(m_pDevice, "data/shader/EdgePS.cso") == false)
	{
		return false;
	}

	//リムライト
	m_PixelShader[LIM] = new CPixel;
	if (m_PixelShader[LIM]->Create(m_pDevice, "data/shader/LimLight.cso") == false)
	{
		return false;
	}

	//2D
	m_PixelShader[PIXEL2D] = new CPixel;
	if (m_PixelShader[PIXEL2D]->Create(m_pDevice, "data/shader/Pixel2D.cso") == false)
	{
		return false;
	}

	//ディソルブ
	m_PixelShader[DISSOLVE] = new CPixel;
	if (m_PixelShader[DISSOLVE]->Create(m_pDevice, "data/shader/DissolvePS.cso") == false)
	{
		return false;
	}

	//バンプマップ
	m_PixelShader[BUMPMAP] = new CPixel;
	if (m_PixelShader[BUMPMAP]->Create(m_pDevice, "data/shader/BumpMap.cso") == false)
	{
		return false;
	}

	//フォグ
	m_PixelShader[FOG] = new CPixel;
	if (m_PixelShader[FOG]->Create(m_pDevice, "data/shader/Fog.cso") == false)
	{
		return false;
	}

	//トゥーン
	m_PixelShader[TOONPS] = new CPixel;
	if (m_PixelShader[TOONPS]->Create(m_pDevice, "data/shader/ToonPS.cso") == false)
	{
		return false;
	}
	
	//影
	m_PixelShader[SHADOWPS] = new CPixel;
	if (m_PixelShader[SHADOWPS]->Create(m_pDevice, "data/shader/ShadowPS.cso") == false)
	{
		return false;
	}

	//シャドウマップ
	m_PixelShader[DEPTHWRITEPS] = new CPixel;
	if (m_PixelShader[DEPTHWRITEPS]->Create(m_pDevice, "data/shader/DepthWritePS.cso") == false)
	{
		return false;
	}

	//深度シャドウ
	m_PixelShader[DEPTHSHADOWPS] = new CPixel;
	if (m_PixelShader[DEPTHSHADOWPS]->Create(m_pDevice, "data/shader/DepthShadowPS.cso") == false)
	{
		return false;
	}

	//モノクロ
	m_PixelShader[MONOCHROME_EFFECT] = new CPixel;
	if (m_PixelShader[MONOCHROME_EFFECT]->Create(m_pDevice, "data/shader/PP_Monochrome.cso") == false)
	{
		return false;
	}

	//シンプルノイズ
	m_PixelShader[NOIZ_EFFECT] = new CPixel;
	if (m_PixelShader[NOIZ_EFFECT]->Create(m_pDevice, "data/shader/PP_Noiz.cso") == false)
	{
		return false;
	}

	//ビネット
	m_PixelShader[VIGNETTE_EFFECT] = new CPixel;
	if (m_PixelShader[VIGNETTE_EFFECT]->Create(m_pDevice, "data/shader/PP_Vignette.cso") == false)
	{
		return false;
	}
	
	//グリッチノイズ
	m_PixelShader[GLITCH_EFFECT] = new CPixel;
	if (m_PixelShader[GLITCH_EFFECT]->Create(m_pDevice, "data/shader/PP_Glitch.cso") == false)
	{
		return false;
	}

	return true;
}

/******************************************************************************
* 
* @brief      CreateTextureSampler
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       テクスチャサンプラ作成(成功でS_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateTextureSampler(void)
{
	D3D11_SAMPLER_DESC sampler_desc;

	ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));

	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// サンプリング時の補間方法
	// UVW値が0.0～1.0の範囲外になった場合の対応設定
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	if (FAILED(m_pDevice->CreateSamplerState(&sampler_desc, &m_pSamplerState)))
	{
		return false;
	}

	return true;
}

/******************************************************************************
* 
* @brief      SetUpViewPort
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ビューポートセッティング
* @attention  
******************************************************************************/
void CBackBuffer::SetUpViewPort(void)
{
	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (float)SCREEN_WIDTH;
	vp.Height = (float)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);
}
/******************************************************************************
* 
* @brief      SetUpViewPort
* @param[in]  x
* @param[in]  y
* @param[in]  width
* @param[in]  height
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ビューポートセッティング(サイズ指定)
* @attention  
******************************************************************************/
void CBackBuffer::SetUpViewPort(float x, float y, float width, float height)
{
	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = (float)x;
	vp.TopLeftY = (float)y;
	m_pDeviceContext->RSSetViewports(1, &vp);
}

/******************************************************************************
* 
* @brief      Release
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       リソースの開放
* @attention  
******************************************************************************/
void CBackBuffer::Release(void)
{
	if (m_pDeviceContext) {
		m_pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}

	//デプスステンシル開放
	SAFE_RELEASE(m_pDepthStencilView);
	//デプスステンシルテクスチャ開放
	SAFE_RELEASE(m_pDepthStencilTexture);
	//レンダ―ターゲットテクスチャ開放
	SAFE_RELEASE(m_pRenderTargetTexture);
	//レンダ―ターゲットビュー開放
	SAFE_RELEASE(m_pRenderTargetView);
	// スワップチェーン解放
	SAFE_RELEASE(m_pSwapChain);
	// デバイス コンテキストの開放
	SAFE_RELEASE(m_pDeviceContext);
	// デバイスの開放
	SAFE_RELEASE(m_pDevice);
}

/******************************************************************************
* 
* @brief      StartRendering
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       描画開始処理
* @attention  
******************************************************************************/
void CBackBuffer::StartRendering(void)
{
	// バックバッファ＆Ｚバッファのクリア
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

/******************************************************************************
* 
* @brief      FinishRendering
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       描画終了処理
* @attention  
******************************************************************************/
void CBackBuffer::FinishRendering(void)
{
	// バックバッファとフロントバッファの入れ替え
	m_pSwapChain->Present(0, 0);
}

/******************************************************************************
* 
* @brief      CreateBlendState
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ブレンドステート作成(成功でS_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateBlendState(void)
{
	// ブレンド ステート生成
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[0]);
	// ブレンド ステート生成 (アルファ ブレンド用)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[1]);
	// ブレンド ステート生成 (加算合成用)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[2]);
	// ブレンド ステート生成 (減算合成用)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[3]);
	SetBlendState(BS_ALPHABLEND);

	return S_OK;
}

/******************************************************************************
* 
* @brief      CreateRasterizerState
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ラスタライズ作成(成功でS_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateRasterizerState()
{

	// ラスタライズ設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;	// カリング無し(両面描画)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[0]);
	rd.CullMode = D3D11_CULL_FRONT;	// 前面カリング(裏面描画)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[1]);
	rd.CullMode = D3D11_CULL_BACK;	// 背面カリング(表面描画)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[2]);
	m_pDeviceContext->RSSetState(m_pRs[2]);

	return S_OK;
}

/******************************************************************************
* 
* @brief      CreateDepthStencilState
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       深度ステンシル作成(成功でS_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateDepthStencilState()
{
	// 深度ステンシルステート生成
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_pDevice->CreateDepthStencilState(&dsd, &m_pDSS[0]);
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;
	m_pDevice->CreateDepthStencilState(&dsd2, &m_pDSS[1]);

	return S_OK;
}