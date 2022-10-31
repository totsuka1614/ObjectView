//=============================================================================
//
// ピクセルシェーダ クラス [Pixel.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "Pixel.h"

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
