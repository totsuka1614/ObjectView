/******************************************************************************
* 
* @file      BackBuffer.cpp
* @brief     �o�b�t�@���F�X����
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
// �O���[�o���ϐ�
//*****************************************************************************
CBackBuffer g_backbuffer;	//�C���X�^���X

CBackBuffer* CBackBuffer::m_pBuffer = &g_backbuffer;  // ���݂̃o�b�t�@

/******************************************************************************
* 
* @brief      Init
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ������(������S_OK)
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

	//�X���b�v�`�F�[���쐬		//�t�����g�o�b�t�@�A�o�b�N�o�b�t�@
	hr = CreateDeviceAndSwapChain();
	if (FAILED(hr))
		return hr;

	//�����_�[�^�[�Q�b�g�쐬	//�`�悷�邽�߂̃��\�[�X�̊m��
	hr = CreateRenderTargetView();
	if (FAILED(hr))
		return hr;

	//Z�o�b�t�@/�X�e���V���o�b�t�@�쐬	//���̂̑O���m��
	hr = CreateDepthAndStencilView();
	if (FAILED(hr))
		return hr;

	//�V�F�[�_�쐬
	hr = CreateShader();
	if (FAILED(hr))
		return hr;

	//�u�����h�X�e�[�g�쐬
	hr = CreateBlendState();
	if (FAILED(hr))
		return hr;

	//�r���[�|�[�g�ݒ�
	SetUpViewPort();

	//���X�^���C�U�ݒ�
	CreateRasterizerState();

	//�[�x�o�b�t�@�쐬
	CreateDepthStencilState();

	//�e�X�g�e�N�X�`���쐬
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
* @note       �`��̃Z�b�g�A�b�v
* @attention  �����Ȃ��̏ꍇ�A��{�̕`��ɂȂ�
******************************************************************************/
void CBackBuffer::SetUpContext(VSShaderType VStype,PSShaderType PStype , D3D_PRIMITIVE_TOPOLOGY topology)
{
	// �v���~�e�B�u�̌`����w��
	m_pDeviceContext->IASetPrimitiveTopology(topology);
	// VerteXShader�APixelShader��ݒ�
	m_pDeviceContext->VSSetShader(
		m_VertexShader[VStype]->GetShader(),	// �g�p����VertexShder
		nullptr,
		0);									// ClassInstance�̐�
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
* @note       �s�N�Z���V�F�[�_�ŗ��p����e�N�X�`�����Z�b�g
* @attention  nNumber�̓X���b�g�ԍ�
******************************************************************************/
void CBackBuffer::SetTexture(ID3D11ShaderResourceView* texture,int nNumber)
{
	// Sampler�̐ݒ�
	m_pDeviceContext->PSSetSamplers(
		0,					// �X���b�g�ԍ�
		1,					// �T���v���[�̐�
		&m_pSamplerState);	// ID3D11SamplerState

	// PixelShader�Ŏg�p����e�N�X�`���̐ݒ�
	m_pDeviceContext->PSSetShaderResources(
		nNumber,								// �X���b�g�ԍ�
		1,								// ���\�[�X�̐�
		&texture);						// ID3D11ShaderResourceView
}

/******************************************************************************
* 
* @brief      SetBlendState
* @param[in]  nBlend
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �u�����h�X�e�[�g���Z�b�g
* @attention  EBlendState���Q��
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
* @note       �J�����O�Z�b�g
* @attention  ECullMode���Q��
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
* @note       �[�x�o�b�t�@�L�������̃Z�b�g
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
* @note       �X���b�v�`�F�[���̍쐬(������S_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateDeviceAndSwapChain(void)
{
	HRESULT hr = S_OK;

	// �f�o�C�X�A�X���b�v�`�F�[���̍쐬
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
* @note       �����_�\�^�[�Q�b�g�쐬(������S_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateRenderTargetView(void)
{
	HRESULT hr = S_OK;

	// RenderTargetView�̑ΏۂƂȂ�Buffer�̎擾
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pRenderTargetTexture);
	if (FAILED(hr))
		return hr;

	// Buffer����RenderTargetView�̍쐬
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
* @note       Z�o�b�t�@���X�e���V���쐬(������S_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateDepthAndStencilView()
{
	// Z�o�b�t�@�p�e�N�X�`������
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

	// Z�o�b�t�@�^�[�Q�b�g�r���[����
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture,
		&dsvd, &m_pDepthStencilView);
	if (FAILED(hr)) {
		return hr;
	}

	// �e�^�[�Q�b�g�r���[�������_�[�^�[�Q�b�g�ɐݒ�
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	return hr;
}

/******************************************************************************
* 
* @brief      CreateShader
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �V�F�[�_�쐬(������S_OK)
* @attention  �ǉ�����x�A��������
******************************************************************************/
HRESULT CBackBuffer::CreateShader(void)
{
	//3D
	m_VertexShader[VERTEX] = new CVertex;
	if (m_VertexShader[VERTEX]->Create(m_pDevice, "data/shader/vertex.cso") == false)
	{
		return false;
	}

	//�A�E�g���C��
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

	//�e
	m_VertexShader[SHADOWVS] = new CVertex;
	if (m_VertexShader[SHADOWVS]->Create(m_pDevice, "data/shader/ShadowVS.cso") == false)
	{
		return false;
	}

	//�[�x�V���h�E
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

	//�g�U��
	m_PixelShader[LAMBERT] = new CPixel;
	if (m_PixelShader[LAMBERT]->Create(m_pDevice, "data/shader/lambert.cso") == false)
	{
		return false;
	}

	//���ʔ���
	m_PixelShader[PHONG] = new CPixel;
	if (m_PixelShader[PHONG]->Create(m_pDevice, "data/shader/Phong.cso") == false)
	{
		return false;
	}

	//���C�g�Ȃ�
	m_PixelShader[UNLIT] = new CPixel;
	if (m_PixelShader[UNLIT]->Create(m_pDevice, "data/shader/Unlit.cso") == false)
	{
		return false;
	}

	//�A�E�g���C��
	m_PixelShader[EDGEPS] = new CPixel;
	if (m_PixelShader[EDGEPS]->Create(m_pDevice, "data/shader/EdgePS.cso") == false)
	{
		return false;
	}

	//�������C�g
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

	//�f�B�\���u
	m_PixelShader[DISSOLVE] = new CPixel;
	if (m_PixelShader[DISSOLVE]->Create(m_pDevice, "data/shader/DissolvePS.cso") == false)
	{
		return false;
	}

	//�o���v�}�b�v
	m_PixelShader[BUMPMAP] = new CPixel;
	if (m_PixelShader[BUMPMAP]->Create(m_pDevice, "data/shader/BumpMap.cso") == false)
	{
		return false;
	}

	//�t�H�O
	m_PixelShader[FOG] = new CPixel;
	if (m_PixelShader[FOG]->Create(m_pDevice, "data/shader/Fog.cso") == false)
	{
		return false;
	}

	//�g�D�[��
	m_PixelShader[TOONPS] = new CPixel;
	if (m_PixelShader[TOONPS]->Create(m_pDevice, "data/shader/ToonPS.cso") == false)
	{
		return false;
	}
	
	//�e
	m_PixelShader[SHADOWPS] = new CPixel;
	if (m_PixelShader[SHADOWPS]->Create(m_pDevice, "data/shader/ShadowPS.cso") == false)
	{
		return false;
	}

	//�V���h�E�}�b�v
	m_PixelShader[DEPTHWRITEPS] = new CPixel;
	if (m_PixelShader[DEPTHWRITEPS]->Create(m_pDevice, "data/shader/DepthWritePS.cso") == false)
	{
		return false;
	}

	//�[�x�V���h�E
	m_PixelShader[DEPTHSHADOWPS] = new CPixel;
	if (m_PixelShader[DEPTHSHADOWPS]->Create(m_pDevice, "data/shader/DepthShadowPS.cso") == false)
	{
		return false;
	}

	//���m�N��
	m_PixelShader[MONOCHROME_EFFECT] = new CPixel;
	if (m_PixelShader[MONOCHROME_EFFECT]->Create(m_pDevice, "data/shader/PP_Monochrome.cso") == false)
	{
		return false;
	}

	//�V���v���m�C�Y
	m_PixelShader[NOIZ_EFFECT] = new CPixel;
	if (m_PixelShader[NOIZ_EFFECT]->Create(m_pDevice, "data/shader/PP_Noiz.cso") == false)
	{
		return false;
	}

	//�r�l�b�g
	m_PixelShader[VIGNETTE_EFFECT] = new CPixel;
	if (m_PixelShader[VIGNETTE_EFFECT]->Create(m_pDevice, "data/shader/PP_Vignette.cso") == false)
	{
		return false;
	}
	
	//�O���b�`�m�C�Y
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
* @note       �e�N�X�`���T���v���쐬(������S_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateTextureSampler(void)
{
	D3D11_SAMPLER_DESC sampler_desc;

	ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));

	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// �T���v�����O���̕�ԕ��@
	// UVW�l��0.0�`1.0�͈̔͊O�ɂȂ����ꍇ�̑Ή��ݒ�
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
* @note       �r���[�|�[�g�Z�b�e�B���O
* @attention  
******************************************************************************/
void CBackBuffer::SetUpViewPort(void)
{
	// �r���[�|�[�g�ݒ�
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
* @note       �r���[�|�[�g�Z�b�e�B���O(�T�C�Y�w��)
* @attention  
******************************************************************************/
void CBackBuffer::SetUpViewPort(float x, float y, float width, float height)
{
	// �r���[�|�[�g�ݒ�
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
* @note       ���\�[�X�̊J��
* @attention  
******************************************************************************/
void CBackBuffer::Release(void)
{
	if (m_pDeviceContext) {
		m_pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}

	//�f�v�X�X�e���V���J��
	SAFE_RELEASE(m_pDepthStencilView);
	//�f�v�X�X�e���V���e�N�X�`���J��
	SAFE_RELEASE(m_pDepthStencilTexture);
	//�����_�\�^�[�Q�b�g�e�N�X�`���J��
	SAFE_RELEASE(m_pRenderTargetTexture);
	//�����_�\�^�[�Q�b�g�r���[�J��
	SAFE_RELEASE(m_pRenderTargetView);
	// �X���b�v�`�F�[�����
	SAFE_RELEASE(m_pSwapChain);
	// �f�o�C�X �R���e�L�X�g�̊J��
	SAFE_RELEASE(m_pDeviceContext);
	// �f�o�C�X�̊J��
	SAFE_RELEASE(m_pDevice);
}

/******************************************************************************
* 
* @brief      StartRendering
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �`��J�n����
* @attention  
******************************************************************************/
void CBackBuffer::StartRendering(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
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
* @note       �`��I������
* @attention  
******************************************************************************/
void CBackBuffer::FinishRendering(void)
{
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pSwapChain->Present(0, 0);
}

/******************************************************************************
* 
* @brief      CreateBlendState
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �u�����h�X�e�[�g�쐬(������S_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateBlendState(void)
{
	// �u�����h �X�e�[�g����
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
	// �u�����h �X�e�[�g���� (�A���t�@ �u�����h�p)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[1]);
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[2]);
	// �u�����h �X�e�[�g���� (���Z�����p)
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
* @note       ���X�^���C�Y�쐬(������S_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateRasterizerState()
{

	// ���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;	// �J�����O����(���ʕ`��)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[0]);
	rd.CullMode = D3D11_CULL_FRONT;	// �O�ʃJ�����O(���ʕ`��)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[1]);
	rd.CullMode = D3D11_CULL_BACK;	// �w�ʃJ�����O(�\�ʕ`��)
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
* @note       �[�x�X�e���V���쐬(������S_OK)
* @attention  
******************************************************************************/
HRESULT CBackBuffer::CreateDepthStencilState()
{
	// �[�x�X�e���V���X�e�[�g����
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_pDevice->CreateDepthStencilState(&dsd, &m_pDSS[0]);
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;
	m_pDevice->CreateDepthStencilState(&dsd2, &m_pDSS[1]);

	return S_OK;
}