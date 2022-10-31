//=============================================================================
//
// シェーダべ―ス クラス [ShaderBase.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include <stdio.h>
#include "ShaderBase.h"

//シェーダ作成
bool ShaderBase::Create(ID3D11Device* device, const char* file_name)
{
	m_Size = LoadFile(file_name);
	if (m_Size == 0)
	{
		return false;
	}
	return true;
}

//シェーダ読み込み
int ShaderBase::LoadFile(const char* file_name)
{
	// シェーダファイル読み込み
	FILE *fp = nullptr;
	fopen_s(&fp, file_name, "rb");

	if (fp == nullptr)
	{
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	m_Size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	m_Data = new char[m_Size];
	fread_s(m_Data, m_Size, m_Size, 1, fp);
	fclose(fp);

	return m_Size;
}
