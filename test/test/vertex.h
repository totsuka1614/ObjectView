/******************************************************************************
* 
* @file      vertex.h
* @brief     ���_�V�F�[�_�N���X
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#ifndef VERTEX_H
#define VERTEX_H
//�C���N���[�h��
#include "main.h"
#include "shaderbase.h"

class CVertex : public ShaderBase
{
public:
	//�R���X�g���N�^
	CVertex() : shader(nullptr) {}
	//�f�X�g���N�^
	~CVertex() { SAFE_RELEASE(shader); }

	//���_�V�F�[�_�쐬�֐�
	virtual bool Create(ID3D11Device* device, const char* file_name);

	ID3D11VertexShader* GetShader() { return shader; }

private:
	ID3D11VertexShader* shader;	//�V�F�[�_�|�C���^
};

#endif