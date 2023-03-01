#include "Filter.h"
#include "Input.h"
#include <algorithm>

void CFilter::Update()
{
	if (m_fTime >= 60.0f)
		m_fTime = 0.0f;

	m_fTime++;

	if (CInput::GetKeyPress(VK_CONTROL))
	{
		if (CInput::GetKeyTrigger(VK_T))
		{
			m_nNumber++;
			if (m_nNumber >= 5)
				m_nNumber = 0;
		}
	}

	switch (m_nNumber)
	{
	case 0:
		m_eType = PIXEL2D;
		break;
	case 1:
		m_eType = MONOCHROME_EFFECT;
		break;
	case 2:
		m_eType = NOIZ_EFFECT;
		break;
	case 3:
		m_eType = VIGNETTE_EFFECT;
		if (CInput::GetKeyPress(VK_LEFT))
			m_fVignette -= 0.05f;
		if (CInput::GetKeyPress(VK_RIGHT))
			m_fVignette += 0.05f;
		m_fVignette = min(m_fVignette, 1.0f);
		m_fVignette = max(m_fVignette, 0.0f);

		break;
	case 4:
		m_eType = GLITCH_EFFECT;
		break;
	default:
		break;
	}
}

void CFilter::Draw()
{
	ID3D11DeviceContext* pDeviceContext = BACKBUFFER->GetDeviceContext();

	BACKBUFFER->SetUpContext(VERTEX2D, m_eType, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 拡縮
	XMMATRIX mWorld = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	// 回転
	mWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vDegree.x),
		XMConvertToRadians(m_vDegree.y), XMConvertToRadians(m_vDegree.z));
	// 移動
	mWorld *= XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	// ワールド マトリックスに設定
	XMStoreFloat4x4(&m_mWorld, mWorld);

	if (m_pTexture) {
		// 拡縮
		mWorld = XMMatrixScaling(m_vSizeTexFrame.x, m_vSizeTexFrame.y, 1.0f);
		// 移動
		mWorld *= XMMatrixTranslation(m_vPosTexFrame.x, m_vPosTexFrame.y, 0.0f);
		// テクスチャ マトリックスに設定
		XMStoreFloat4x4(&m_mTex, mWorld);
	}
	else {
		// テクスチャ無し
		m_mTex._44 = 0.0f;
	}

	// 頂点バッファ更新
	SetVertex();

	pDeviceContext->IASetInputLayout(m_pInputLayout);

	UINT stride = sizeof(VERTEX_2D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);


	BACKBUFFER->SetTexture(m_pTexture);

	SHADER_GLOBAL cb;
	cb.mProj = XMMatrixTranspose(XMLoadFloat4x4(&m_mProj));
	cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&m_mView));
	cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_mWorld));
	cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(&m_mTex));
	cb.bColor = m_fColor;
	cb.rand = rand() % 100 + 50;
	cb.vignette = m_fVignette;
	cb.VertGlitchPase  = rand() % 100 *0.01f;
	cb.HorzGlitchPase  = rand() % 100 *0.01f;
	cb.GlitchStepValue = m_fVignette;
	cb.time = m_fTime;
	
	m_pConstantBuffer->Update(&cb);
	m_pConstantBuffer->SetVertexShader();
	m_pConstantBuffer->SetPixelShader();

	// ポリゴンの描画
	pDeviceContext->Draw(NUM_VERTEX, 0);
}