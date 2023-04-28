/******************************************************************************
* 
* @file      shaderbase.h
* @brief     �V�F�[�_�ׁ\�X
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#ifndef SHADERBASE_H
#define SHADERBASE_H
//�C���N���[�h��
#include "main.h"

class ShaderBase
{
public:
	//�R���X�g���N�^
	ShaderBase() :m_Size(0), m_Data(nullptr) {}
	//�f�X�g���N�^
	virtual ~ShaderBase() { SAFE_DELETE(m_Data); }

	bool Create(ID3D11Device* device, const char* file_name);

	char* GetData() { return m_Data; }
	long GetSize() { return m_Size; }
	

private:
	int LoadFile(const char* file_name);

protected:

	char* m_Data;
	long m_Size;
};

#endif // !SHADERBASE_H
