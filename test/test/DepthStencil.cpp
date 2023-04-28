/******************************************************************************
* 
* @file      DepthStencil.cpp
* @brief     �[�x�X�e���V��
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "DepthStencil.h"
#include "BackBuffer.h"

/******************************************************************************
* 
* @brief      DepthStencil
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �R���X�g���N�^
* @attention  
******************************************************************************/
CDepthStencil::CDepthStencil() :m_pDSV(nullptr), m_pTex(nullptr), m_pSRV(nullptr), m_fHeight(0.0f), m_fWidth(0.0f)
{

}

/******************************************************************************
* 
* @brief      ~DepthStencil
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �f�X�g���N�^
* @attention  
******************************************************************************/
CDepthStencil::~CDepthStencil()
{
	//�f�v�X�X�e���V���J��
	SAFE_RELEASE(m_pDSV);
	//�e�N�X�`���J��
	SAFE_RELEASE(m_pTex);
	//���\�[�X�r���[�J��
	SAFE_RELEASE(m_pSRV);
}

/******************************************************************************
* 
* @brief      Create
* @param[in]  format
* @param[in]  width
* @param[in]  height
* @param[in]  pData
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �萔�o�b�t�@�쐬
* @attention  
******************************************************************************/
HRESULT CDepthStencil::Create(DXGI_FORMAT format, float width, float height, const void* pData)
{
	HRESULT hr;

	// �X�e���V���g�p����
	bool useStencil = (format == DXGI_FORMAT_D24_UNORM_S8_UINT);

	// ���\�[�X����
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

	// �e�N�X�`���쐬
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = pData;
	data.SysMemPitch = desc.Width * 4;
	hr = BACKBUFFER->GetDevice()->CreateTexture2D(&desc, pData ? &data : nullptr, &m_pTex);

	if (FAILED(hr))
		return hr;


	// �ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = BACKBUFFER->GetDevice()->CreateShaderResourceView(m_pTex, &srvDesc, &m_pSRV);

	m_fWidth = width;
	m_fHeight = height;

	// �ݒ�
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	// ����
	return BACKBUFFER->GetDevice()->CreateDepthStencilView(m_pTex, &dsvDesc, &m_pDSV);
}