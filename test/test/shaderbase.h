//=============================================================================
//
// シェーダ―ベース クラス定義 [ShaderBase.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef SHADERBASE_H
#define SHADERBASE_H

#include "main.h"

class ShaderBase
{
public:
	//コンストラクタ
	ShaderBase() :m_Size(0), m_Data(nullptr) {}
	//デストラクタ
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
