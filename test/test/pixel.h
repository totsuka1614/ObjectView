//=============================================================================
//
// ピクセルシェーダ クラス定義 [Pixel.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef PIXEL_H
#define PIXEL_H

#include "main.h"
#include "shaderbase.h"

class Pixel : public ShaderBase
{
public:
	//コンストラクタ
	Pixel() : shader(nullptr) {}
	//デストラクタ
	~Pixel() { SAFE_RELEASE(shader); }

	//ピクセルシェーダ作成関数
	virtual bool Create(ID3D11Device* device,const char* file_name);

	ID3D11PixelShader* GetShader() { return shader; }

private:
	ID3D11PixelShader* shader;	//シェーダポインタ
};

#endif