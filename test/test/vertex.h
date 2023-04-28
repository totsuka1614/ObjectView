/******************************************************************************
* 
* @file      vertex.h
* @brief     頂点シェーダクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#ifndef VERTEX_H
#define VERTEX_H
//インクルード部
#include "main.h"
#include "shaderbase.h"

class CVertex : public ShaderBase
{
public:
	//コンストラクタ
	CVertex() : shader(nullptr) {}
	//デストラクタ
	~CVertex() { SAFE_RELEASE(shader); }

	//頂点シェーダ作成関数
	virtual bool Create(ID3D11Device* device, const char* file_name);

	ID3D11VertexShader* GetShader() { return shader; }

private:
	ID3D11VertexShader* shader;	//シェーダポインタ
};

#endif