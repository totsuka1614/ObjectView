/******************************************************************************
* 
* @file      vertex.cpp
* @brief     頂点シェーダ
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "vertex.h"

/******************************************************************************
* 
* @brief      Create
* @param[in]  device
* @param[in]  file_name
* @return     bool
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       頂点シェーダ作成(成功でtrue)
* @attention  
******************************************************************************/
bool Vertex::Create(ID3D11Device* device, const char* file_name)
{
	//シェーダ読み込み
	if (ShaderBase::Create(device, file_name) == false)
	{
		return false;
	}

	//ピクセルシェーダ作成
	if (FAILED(device->CreateVertexShader(m_Data, m_Size, nullptr, &shader)))
	{
		return false;
	}

	return true;
}
