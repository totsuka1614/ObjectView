//=============================================================================
//
// �萔�o�b�t�@ �N���X��` [ConstantBuffer.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef __CONSTANTBUFFER_H__
#define __CONSTANTBUFFER_H__


#include "main.h"

//�萔�o�b�t�@��ێ�
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