//=============================================================================
//
// �s�N�Z���V�F�[�_ �N���X [Pixel.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "Pixel.h"

bool Pixel::Create(ID3D11Device* device, const char* file_name)
{
	//�V�F�[�_�ǂݍ���
	if (ShaderBase::Create(device, file_name) == false)
	{
		return false;
	}

	//�s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(m_Data, m_Size, nullptr, &shader)))
	{
		return false;
	}

	return true;
}
