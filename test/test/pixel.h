/******************************************************************************
* 
* @file      pixel.h
* @brief     �s�N�Z���V�F�[�_�N���X
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#ifndef PIXEL_H
#define PIXEL_H
//�C���N���[�h��
#include "main.h"
#include "shaderbase.h"

class CPixel : public ShaderBase
{
public:
	//�R���X�g���N�^
	CPixel() : shader(nullptr) {}
	//�f�X�g���N�^
	~CPixel() { SAFE_RELEASE(shader); }

	//�s�N�Z���V�F�[�_�쐬�֐�
	virtual bool Create(ID3D11Device* device,const char* file_name);

	ID3D11PixelShader* GetShader() { return shader; }

private:
	ID3D11PixelShader* shader;	//�V�F�[�_�|�C���^
};

#endif