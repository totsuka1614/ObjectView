//=============================================================================
//
// �萔�o�b�t�@ �N���X [ConstantBuffer.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "ConstantBuffer.h"
#include "BackBuffer.h"

HRESULT ConstantBuffer::Create(UINT size)
{
	/*
	Constant�o�b�t�@�[�쐬
		�R���X�^���g�o�b�t�@�[��CPU���̃f�[�^��
		�܂Ƃ߂�GPU���ɑ��M���邽�߂̃o�b�t�@�[

		�o�b�t�@�[�ɂ͍��W�ϊ��s��Ȃǂ�ݒ肷��
*/
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = size;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	HRESULT hr;
	
	hr = BackBuffer::GetBuffer()->GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_pConstantBuffer);

	if (FAILED(hr))
		return hr;

	return hr;
}

void ConstantBuffer::Update(const void* pData)
{
	// �萔�o�b�t�@�ւ̏�������
	ID3D11DeviceContext *bb = BackBuffer::GetBuffer()->GetDeviceContext();
		
	bb->UpdateSubresource(
		m_pConstantBuffer, 0, nullptr, pData, 0, 0
	);
}

void ConstantBuffer::SetVertexShader(void)
{
	BackBuffer::GetBuffer()->GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
}

void ConstantBuffer::SetPixelShader(int nSlot)
{
	BackBuffer::GetBuffer()->GetDeviceContext()->PSSetConstantBuffers(nSlot, 1, &m_pConstantBuffer);
}