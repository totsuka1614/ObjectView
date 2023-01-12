//=============================================================================
//
// 定数バッファ クラス定義 [ConstantBuffer.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef __CONSTANTBUFFER_H__
#define __CONSTANTBUFFER_H__


#include "main.h"

//定数バッファを保持
class ConstantBuffer
{
public:
	ConstantBuffer() : m_pConstantBuffer(nullptr){}
	~ConstantBuffer() {}

	HRESULT Create(UINT);
	void Update(const void*);
	ID3D11Buffer* GetConstantBuffer(void) { return m_pConstantBuffer; }
	void SetVertexShader(int nSlot = 0);
	void SetPixelShader(int nSlot = 0);


private:
	ID3D11Buffer* m_pConstantBuffer;
};
#endif // !__CONSTANTBUFFER_H__