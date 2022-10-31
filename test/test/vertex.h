//=============================================================================
//
// 頂点シェーダ クラス定義 [vertex.h]
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
	//コンストラクタ
	Vertex() : shader(nullptr) {}
	//デストラクタ
	~Vertex() { SAFE_RELEASE(shader); }

	//頂点シェーダ作成関数
	virtual bool Create(ID3D11Device* device, const char* file_name);

	ID3D11VertexShader* GetShader() { return shader; }

private:
	ID3D11VertexShader* shader;	//シェーダポインタ
};

#endif