#include "billboard.h"
#include "Camera.h"

void CBillboard::Init()
{
	CPolygon::Init(); 
}

void CBillboard::Draw()
{
	ID3D11DeviceContext* pDeviceContext = BACKBUFFER->GetDeviceContext();

	BACKBUFFER->SetUpContext(VERTEX2D, PIXEL2D, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();

	XMFLOAT4X4 world;
	
	XMMATRIX mWorld;

	// �r���[�s��̉�]�����̓]�u�s���ݒ�(+�g�k)
	world._11 = mView._11 * m_vScale.x;
	world._12 = mView._21 * m_vScale.x;
	world._13 = mView._31 * m_vScale.x;
	world._14 = 0.0f;
	world._21 = mView._12 * m_vScale.y;
	world._22 = mView._22 * m_vScale.y;
	world._23 = mView._32 * m_vScale.y;
	world._24 = 0.0f;
	world._31 = mView._13;
	world._32 = mView._23;
	world._33 = mView._33;
	world._34 = 0.0f;
	// ��.�𔽉f
	world._41 = m_vPos.x;
	world._42 = m_vPos.y;
	world._43 = m_vPos.z;
	world._44 = 1.0f;

	mWorld = XMLoadFloat4x4(&world);
	m_mWorld = world;

	if (m_pTexture) {
		// �g�k
		mWorld = XMMatrixScaling(m_vSizeTexFrame.x, m_vSizeTexFrame.y, 1.0f);
		// �ړ�
		mWorld *= XMMatrixTranslation(m_vPosTexFrame.x, m_vPosTexFrame.y, 0.0f);
		// �e�N�X�`�� �}�g���b�N�X�ɐݒ�
		XMStoreFloat4x4(&m_mTex, mWorld);
	}
	else {
		// �e�N�X�`������
		m_mTex._44 = 0.0f;
	}

	// ���_�o�b�t�@�X�V
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
	m_pConstantBuffer->Update(&cb);
	m_pConstantBuffer->SetVertexShader();
	m_pConstantBuffer->SetPixelShader();

	// �|���S���̕`��
	pDeviceContext->Draw(NUM_VERTEX, 0);
}