/******************************************************************************
* 
* @file      ConstantBuffer.h
* @brief     �萔�o�b�t�@�N���X
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#ifndef __CONSTANTBUFFER_H__
#define __CONSTANTBUFFER_H__

//�C���N���[�h��
#include "main.h"

//�萔�o�b�t�@��ێ�
class CConstantBuffer
{
public:
	CConstantBuffer() : m_pConstantBuffer(nullptr){}
	~CConstantBuffer() {}

	HRESULT Create(UINT);
	void Update(const void*);
	ID3D11Buffer* GetConstantBuffer(void) { return m_pConstantBuffer; }
	void SetVertexShader(int nSlot = 0);
	void SetPixelShader(int nSlot = 0);


private:
	ID3D11Buffer* m_pConstantBuffer;
};

#endif // !__CONSTANTBUFFER_H__