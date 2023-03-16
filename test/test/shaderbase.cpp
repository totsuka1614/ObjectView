/******************************************************************************
* 
* @file      shaderbase.cpp
* @brief     シェーダべースクラス
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include <stdio.h>
#include "ShaderBase.h"

/******************************************************************************
* 
* @brief      Create
* @param[in]  device
* @param[in]  file_name
* @return     bool
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ファイルがあればtrue
* @attention  
******************************************************************************/
bool ShaderBase::Create(ID3D11Device* device, const char* file_name)
{
	m_Size = LoadFile(file_name);
	if (m_Size == 0)
	{
		return false;
	}
	return true;
}

/******************************************************************************
* 
* @brief      LoadFile
* @param[in]  file_name
* @return     int
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       シェーダファイルをロード
* @attention  
******************************************************************************/
int ShaderBase::LoadFile(const char* file_name)
{
	// シェーダファイル読み込み
	FILE *fp = nullptr;
	fopen_s(&fp, file_name, "rb");

	//ファイルが見つからない
	if (fp == nullptr)
	{
		return 0;
	}

	//データサイズ取得
	fseek(fp, 0, SEEK_END);		//データ末尾に移動
	m_Size = ftell(fp);			//サイズ取得
	fseek(fp, 0, SEEK_SET);		//先頭へ戻す
	m_Data = new char[m_Size];

	//ロード
	fread_s(m_Data, m_Size, m_Size, 1, fp);
	fclose(fp);

	return m_Size;
}
