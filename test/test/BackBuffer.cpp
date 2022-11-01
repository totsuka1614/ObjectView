//=============================================================================
//
// �o�b�N�o�b�t�@ �N���X [BackBuffer.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "BackBuffer.h"
#include "window.h"

#pragma comment(lib, "d3d11")

BackBuffer g_backbuffer;

BackBuffer* BackBuffer::m_pBuffer = &g_backbuffer;


//������
HRESULT BackBuffer::Init(void)
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

	hr = CreateShader();
	if (FAILED(hr))
		return hr;

	SetUpViewPort();

	return hr;
}




void BackBuffer::SetUpContext(VSShaderType VStype,PSShaderType PStype , D3D_PRIMITIVE_TOPOLOGY topology)
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

	// (OutputManger)RenderTarget�̎w��
	m_pDeviceContext->OMSetRenderTargets(
		1,							// �g�p����View�̐�
		&m_pRenderTargetView,		// �g�p����RenderTargetView
		m_pDepthStencilView);		// �g�p����DepthStencilView
}

void BackBuffer::SetTexture(ID3D11ShaderResourceView* texture)
{
	// Sampler�̐ݒ�
	m_pDeviceContext->PSSetSamplers(
		0,					// �X���b�g�ԍ�
		1,					// �T���v���[�̐�
		&m_pSamplerState);	// ID3D11SamplerState

	// PixelShader�Ŏg�p����e�N�X�`���̐ݒ�
	m_pDeviceContext->PSSetShaderResources(
		0,								// �X���b�g�ԍ�
		1,								// ���\�[�X�̐�
		&texture);						// ID3D11ShaderResourceView
}

//�X���b�v�`�F�[���쐬
HRESULT BackBuffer::CreateDeviceAndSwapChain(void)
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

//�����_�\�^�[�Q�b�g�쐬
HRESULT BackBuffer::CreateRenderTargetView(void)
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* back_buffer;

	// RenderTargetView�̑ΏۂƂȂ�Buffer�̎擾
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);
	if (FAILED(hr))
		return hr;

	hr = m_pDevice->CreateRenderTargetView(back_buffer, NULL, &m_pRenderTargetView);
	// Buffer����RenderTargetView�̍쐬
	if (FAILED(hr))
		return hr;

	// Target�̎擾�I������̂�Buffer�����
	back_buffer->Release();

	return hr;
}

//Z�o�b�t�@/�X�e���V���o�b�t�@�쐬
HRESULT BackBuffer::CreateDepthAndStencilView()
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

	return true;
}

HRESULT BackBuffer::CreateTextureSampler(void)
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

//�r���[�|�b�g�ݒ�
void BackBuffer::SetUpViewPort(void)
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

//���\�[�X�̊J��
void BackBuffer::Release(void)
{
	if (m_pDeviceContext) {
		m_pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}

	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilTexture);
	SAFE_RELEASE(m_pRenderTargetView);

	// �X���b�v�`�F�[�����
	SAFE_RELEASE(m_pSwapChain);

	// �f�o�C�X �R���e�L�X�g�̊J��
	SAFE_RELEASE(m_pDeviceContext);

	// �f�o�C�X�̊J��
	SAFE_RELEASE(m_pDevice);
}

//�`��X�^�[�g����
void BackBuffer::StartRendering(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

//�`��G���h����
void BackBuffer::FinishRendering(void)
{
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
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