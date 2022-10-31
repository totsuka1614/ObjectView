//=============================================================================
//
// ���_�V�F�[�_ �N���X��` [vertex.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef VERTEX_H
#define VERTEX_H

#include "main.h"
#include "shaderbase.h"

class Vertex : public ShaderBase
{
public:
	//�R���X�g���N�^
	Vertex() : shader(nullptr) {}
	//�f�X�g���N�^
	~Vertex() { SAFE_RELEASE(shader); }

	//���_�V�F�[�_�쐬�֐�
	virtual bool Create(ID3D11Device* device, const char* file_name);

	ID3D11VertexShader* GetShader() { return shader; }

private:
	ID3D11VertexShader* shader;	//�V�F�[�_�|�C���^
};

#endif