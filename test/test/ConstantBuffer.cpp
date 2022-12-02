//=============================================================================
//
// 定数バッファ クラス [ConstantBuffer.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "ConstantBuffer.h"
#include "BackBuffer.h"

HRESULT ConstantBuffer::Create(UINT size)
{
	/*
	Constantバッファー作成
		コンスタントバッファーはCPU側のデータを
		まとめてGPU側に送信するためのバッファー

		バッファーには座標変換行列などを設定する
*/
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = size;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	HRESULT hr;
	
	hr = BackBuffer::GetBuffer()->GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_pConstantBuffer);

	if (FAILED(hr))
		return hr;

	return hr;
}

void ConstantBuffer::Update(const void* pData)
{
	// 定数バッファへの書き込み
	ID3D11DeviceContext *bb = BackBuffer::GetBuffer()->GetDeviceContext();
		
	bb->UpdateSubresource(
		m_pConstantBuffer, 0, nullptr, pData, 0, 0
	);
}

void ConstantBuffer::SetVertexShader(void)
{
	BackBuffer::GetBuffer()->GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
}

void ConstantBuffer::SetPixelShader(int nSlot)
{
	BackBuffer::GetBuffer()->GetDeviceContext()->PSSetConstantBuffers(nSlot, 1, &m_pConstantBuffer);
}