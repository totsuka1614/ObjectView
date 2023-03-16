/******************************************************************************
* 
* @file      ConstantBuffer.cpp
* @brief     定数バッファ
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "ConstantBuffer.h"
#include "BackBuffer.h"

/******************************************************************************
* 
* @brief      Create
* @param[in]  size
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       定数バッファ作成
* @attention  
******************************************************************************/
HRESULT ConstantBuffer::Create(UINT size)
{
	/*
	定数バッファ作成
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
	
	//作成
	hr = BACKBUFFER->GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_pConstantBuffer);

	if (FAILED(hr))
		return hr;

	return hr;
}

/******************************************************************************
* 
* @brief      Update
* @param[in]  pData
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       更新処理
* @attention  
******************************************************************************/
void ConstantBuffer::Update(const void* pData)
{
	// 定数バッファへの書き込み
	ID3D11DeviceContext *bb = BACKBUFFER->GetDeviceContext();
	
	//更新
	bb->UpdateSubresource(
		m_pConstantBuffer, 0, nullptr, pData, 0, 0
	);
}

/******************************************************************************
* 
* @brief      SetVertexShader
* @param[in]  nSlot
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       頂点シェーダ設定
* @attention  
******************************************************************************/
void ConstantBuffer::SetVertexShader(int nSlot)
{
	BACKBUFFER->GetDeviceContext()->VSSetConstantBuffers(nSlot, 1, &m_pConstantBuffer);
}

/******************************************************************************
* 
* @brief      SetPixelShader
* @param[in]  nSlot
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ピクセルシェーダ設定
* @attention  
******************************************************************************/
void ConstantBuffer::SetPixelShader(int nSlot)
{
	BACKBUFFER->GetDeviceContext()->PSSetConstantBuffers(nSlot, 1, &m_pConstantBuffer);
}