/******************************************************************************
* 
* @file      pixel.cpp
* @brief     ピクセルシェーダ
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
* @note       ピクセルシェーダを作成(成功でtrue)
* @attention  
******************************************************************************/
bool Pixel::Create(ID3D11Device* device, const char* file_name)
{
	//シェーダ読み込み
	if (ShaderBase::Create(device, file_name) == false)
	{
		return false;
	}

	//ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(m_Data, m_Size, nullptr, &shader)))
	{
		return false;
	}

	return true;
}
