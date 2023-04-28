/******************************************************************************
* 
* @file      pixel.h
* @brief     ピクセルシェーダクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#ifndef PIXEL_H
#define PIXEL_H
//インクルード部
#include "main.h"
#include "shaderbase.h"

class CPixel : public ShaderBase
{
public:
	//コンストラクタ
	CPixel() : shader(nullptr) {}
	//デストラクタ
	~CPixel() { SAFE_RELEASE(shader); }

	//ピクセルシェーダ作成関数
	virtual bool Create(ID3D11Device* device,const char* file_name);

	ID3D11PixelShader* GetShader() { return shader; }

private:
	ID3D11PixelShader* shader;	//シェーダポインタ
};

#endif