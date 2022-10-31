//=============================================================================
//
// �s�N�Z���V�F�[�_ �N���X��` [Pixel.h]
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
	//�R���X�g���N�^
	Pixel() : shader(nullptr) {}
	//�f�X�g���N�^
	~Pixel() { SAFE_RELEASE(shader); }

	//�s�N�Z���V�F�[�_�쐬�֐�
	virtual bool Create(ID3D11Device* device,const char* file_name);

	ID3D11PixelShader* GetShader() { return shader; }

private:
	ID3D11PixelShader* shader;	//�V�F�[�_�|�C���^
};

#endif