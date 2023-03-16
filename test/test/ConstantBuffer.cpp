/******************************************************************************
* 
* @file      ConstantBuffer.cpp
* @brief     �萔�o�b�t�@
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "ConstantBuffer.h"
#include "BackBuffer.h"

/******************************************************************************
* 
* @brief      Create
* @param[in]  size
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �萔�o�b�t�@�쐬
* @attention  
******************************************************************************/
HRESULT ConstantBuffer::Create(UINT size)
{
	/*
	�萔�o�b�t�@�쐬
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
	
	//�쐬
	hr = BACKBUFFER->GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_pConstantBuffer);

	if (FAILED(hr))
		return hr;

	return hr;
}

/******************************************************************************
* 
* @brief      Update
* @param[in]  pData
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �X�V����
* @attention  
******************************************************************************/
void ConstantBuffer::Update(const void* pData)
{
	// �萔�o�b�t�@�ւ̏�������
	ID3D11DeviceContext *bb = BACKBUFFER->GetDeviceContext();
	
	//�X�V
	bb->UpdateSubresource(
		m_pConstantBuffer, 0, nullptr, pData, 0, 0
	);
}

/******************************************************************************
* 
* @brief      SetVertexShader
* @param[in]  nSlot
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ���_�V�F�[�_�ݒ�
* @attention  
******************************************************************************/
void ConstantBuffer::SetVertexShader(int nSlot)
{
	BACKBUFFER->GetDeviceContext()->VSSetConstantBuffers(nSlot, 1, &m_pConstantBuffer);
}

/******************************************************************************
* 
* @brief      SetPixelShader
* @param[in]  nSlot
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �s�N�Z���V�F�[�_�ݒ�
* @attention  
******************************************************************************/
void ConstantBuffer::SetPixelShader(int nSlot)
{
	BACKBUFFER->GetDeviceContext()->PSSetConstantBuffers(nSlot, 1, &m_pConstantBuffer);
}