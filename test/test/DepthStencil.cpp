#include "DepthStencil.h"
#include "BackBuffer.h"

DepthStencil::DepthStencil() :m_pDSV(nullptr), m_pTex(nullptr), m_pSRV(nullptr), m_fHeight(0.0f), m_fWidth(0.0f)
{

}

DepthStencil::~DepthStencil()
{
	SAFE_RELEASE(m_pDSV);
	SAFE_RELEASE(m_pTex);
	SAFE_RELEASE(m_pSRV);
}

HRESULT DepthStencil::Create(DXGI_FORMAT format, float width, float height, const void* pData)
{
	HRESULT hr;


	// ステンシル使用判定
	bool useStencil = (format == DXGI_FORMAT_D24_UNORM_S8_UINT);

	// リソース生成
	format = useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;

	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;

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
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	// 生成
	return BACKBUFFER->GetDevice()->CreateDepthStencilView(m_pTex, &dsvDesc, &m_pDSV);
}