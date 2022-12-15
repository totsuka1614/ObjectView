#include "RenderTarget.h"
#include "BackBuffer.h"

RenderTarget::RenderTarget() :m_pRTV(nullptr), m_pTex(nullptr), m_pSRV(nullptr), m_fHeight(0.0f),m_fWidth(0.0f)
{

}

RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(m_pRTV);
	SAFE_RELEASE(m_pTex);
	SAFE_RELEASE(m_pSRV);
}

HRESULT RenderTarget::Create(DXGI_FORMAT format, float width, float height, const void* pData)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;

	desc.BindFlags |= D3D11_BIND_RENDER_TARGET;

	// テクスチャ作成
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = pData;
	data.SysMemPitch = desc.Width * 4;
	hr = BACKBUFFER->GetDevice()->CreateTexture2D(&desc, pData ? &data : nullptr, &m_pTex);

	if (FAILED(hr))
		return hr;

	// 設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	
	hr = BACKBUFFER->GetDevice()->CreateShaderResourceView(m_pTex, &srvDesc, &m_pSRV);

	if (FAILED(hr))
		return hr;

	m_fWidth = width;
	m_fHeight = height;

	// 設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = desc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	// 生成
	return BACKBUFFER->GetDevice()->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);
}