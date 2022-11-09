//=============================================================================
//
// バックバッファ クラス [BackBuffer.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "BackBuffer.h"
#include "window.h"

#pragma comment(lib, "d3d11")

BackBuffer g_backbuffer;

BackBuffer* BackBuffer::m_pBuffer = &g_backbuffer;


//初期化
HRESULT BackBuffer::Init(void)
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

	hr = CreateShader();
	if (FAILED(hr))
		return hr;

	hr = CreateBlendState();
	if (FAILED(hr))
		return hr;

	SetUpViewPort();

	return hr;
}




void BackBuffer::SetUpContext(VSShaderType VStype,PSShaderType PStype , D3D_PRIMITIVE_TOPOLOGY topology)
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

	// (OutputManger)RenderTargetの指定
	m_pDeviceContext->OMSetRenderTargets(
		1,							// 使用するViewの数
		&m_pRenderTargetView,		// 使用するRenderTargetView
		m_pDepthStencilView);		// 使用するDepthStencilView
}

void BackBuffer::SetTexture(ID3D11ShaderResourceView* texture)
{
	// Samplerの設定
	m_pDeviceContext->PSSetSamplers(
		0,					// スロット番号
		1,					// サンプラーの数
		&m_pSamplerState);	// ID3D11SamplerState

	// PixelShaderで使用するテクスチャの設定
	m_pDeviceContext->PSSetShaderResources(
		0,								// スロット番号
		1,								// リソースの数
		&texture);						// ID3D11ShaderResourceView
}

//=============================================================================
// ブレンド ステート設定
//=============================================================================
void BackBuffer::SetBlendState(int nBlend)
{
	if (nBlend >= 0 && nBlend < MAX_BLENDSTATE) {
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_pDeviceContext->OMSetBlendState(m_pBlendState[nBlend], blendFactor, 0xffffffff);
	}
}

//スワップチェーン作成
HRESULT BackBuffer::CreateDeviceAndSwapChain(void)
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
	scd.OutputWindow = Window::GetWindow()->GetWindowHandle();
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

//レンダ―ターゲット作成
HRESULT BackBuffer::CreateRenderTargetView(void)
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* back_buffer;

	// RenderTargetViewの対象となるBufferの取得
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);
	if (FAILED(hr))
		return hr;

	hr = m_pDevice->CreateRenderTargetView(back_buffer, NULL, &m_pRenderTargetView);
	// BufferからRenderTargetViewの作成
	if (FAILED(hr))
		return hr;

	// Targetの取得終わったのでBufferを解放
	back_buffer->Release();

	return hr;
}

//Zバッファ/ステンシルバッファ作成
HRESULT BackBuffer::CreateDepthAndStencilView()
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

HRESULT BackBuffer::CreateShader(void)
{
	m_VertexShader[VERTEX] = new Vertex;
	if (m_VertexShader[VERTEX]->Create(m_pDevice, "data/shader/vertex.cso") == false)
	{
		return false;
	}

	m_PixelShader[PIXEL] = new Pixel;
	if (m_PixelShader[PIXEL]->Create(m_pDevice, "data/shader/pixel.cso") == false)
	{
		return false;
	}

	m_PixelShader[LAMBERT] = new Pixel;
	if (m_PixelShader[LAMBERT]->Create(m_pDevice, "data/shader/lambert.cso") == false)
	{
		return false;
	}

	m_PixelShader[PHONG] = new Pixel;
	if (m_PixelShader[PHONG]->Create(m_pDevice, "data/shader/Phong.cso") == false)
	{
		return false;
	}

	m_PixelShader[UNLIT] = new Pixel;
	if (m_PixelShader[UNLIT]->Create(m_pDevice, "data/shader/Unlit.cso") == false)
	{
		return false;
	}

	return true;
}

HRESULT BackBuffer::CreateTextureSampler(void)
{
	D3D11_SAMPLER_DESC sampler_desc;

	ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));

	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// サンプリング時の補間方法
	// UVW値が0.0〜1.0の範囲外になった場合の対応設定
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	if (FAILED(m_pDevice->CreateSamplerState(&sampler_desc, &m_pSamplerState)))
	{
		return false;
	}

	return true;
}

//ビューポット設定
void BackBuffer::SetUpViewPort(void)
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

//リソースの開放
void BackBuffer::Release(void)
{
	if (m_pDeviceContext) {
		m_pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}

	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilTexture);
	SAFE_RELEASE(m_pRenderTargetView);

	// スワップチェーン解放
	SAFE_RELEASE(m_pSwapChain);

	// デバイス コンテキストの開放
	SAFE_RELEASE(m_pDeviceContext);

	// デバイスの開放
	SAFE_RELEASE(m_pDevice);
}

//描画スタート処理
void BackBuffer::StartRendering(void)
{
	// バックバッファ＆Ｚバッファのクリア
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

//描画エンド処理
void BackBuffer::FinishRendering(void)
{
	// バックバッファとフロントバッファの入れ替え
	m_pSwapChain->Present(0, 0);
}

/*
void BackBuffer::DrawPolygon(Model *model)
{
	UINT strides = sizeof(CustomVertex);
	UINT offsets = 0;
	ID3D11Buffer* buff = model->GetBuffer();
	m_pDeviceContext->IASetInputLayout(model->GetInputLayout());
	m_pDeviceContext->IASetVertexBuffers(0, 1, &buff, &strides, &offsets);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->VSSetShader(m_VertexShader->GetShader(), NULL, 0);
	m_pDeviceContext->PSSetShader(m_PixelShader->GetShader(), NULL, 0);
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	m_pDeviceContext->Draw(3, 0);
}
*/

HRESULT BackBuffer::CreateBlendState(void)
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