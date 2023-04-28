/******************************************************************************
* 
* @file      pixel.cpp
* @brief     �s�N�Z���V�F�[�_
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "Pixel.h"

/******************************************************************************
* 
* @brief      Create
* @param[in]  device
* @param[in]  file_name
* @return     bool
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �s�N�Z���V�F�[�_���쐬(������true)
* @attention  
******************************************************************************/
bool CPixel::Create(ID3D11Device* device, const char* file_name)
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
