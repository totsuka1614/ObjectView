//=============================================================================
//
// �萔�o�b�t�@ �N���X��` [ConstantBuffer.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once

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
	void SetVertexShader(void);
	void SetPixelShader(void);


private:
	ID3D11Buffer* m_pConstantBuffer;
};