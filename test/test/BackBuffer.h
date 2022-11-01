//=============================================================================
//
// �o�b�N�o�b�t�@ �N���X��` [BackBuffer.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef BUFFER_H
#define BUFFER_H

#include "main.h"
#include "vertex.h"
#include "pixel.h"
#include "model.h"

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
	void SetTexture(ID3D11ShaderResourceView*);

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
	void SetUpViewPort(void);
private:

	ID3D11Device*				m_pDevice;							//
	ID3D11DeviceContext*		m_pDeviceContext;
	IDXGISwapChain*				m_pSwapChain;			// �X���b�v�`�F�[��
	ID3D11RenderTargetView*		m_pRenderTargetView;	// �t���[���o�b�t�@
	ID3D11Texture2D*			m_pDepthStencilTexture;	// Z�o�b�t�@�p������
	ID3D11DepthStencilView*		m_pDepthStencilView;	// Z�o�b�t�@
	ID3D11DepthStencilState*	m_pDSS[2];				// Z/�X�e���V�� �X�e�[�g
	ID3D11SamplerState*			m_pSamplerState;		//Texture�T���v���[


	//�p�C�v���C���ɓo�^����V�F�[�_
	Vertex* m_VertexShader[MAX_VSSHADER];
	Pixel* m_PixelShader[MAX_PSSHADER];

	static BackBuffer* m_pBuffer;
};

#endif // !BUFFER_H