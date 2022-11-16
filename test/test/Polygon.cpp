//=============================================================================
//
// 2D�|���S�������N���X���� [Polygon.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "Polygon.h"

// �V�F�[�_�ɓn���l
struct SHADER_GLOBAL {
	XMMATRIX	mWorld;		// ���[���h�ϊ��s��(�]�u�s��)
	XMMATRIX	mView;		// �r���[�ϊ��s��(�]�u�s��)
	XMMATRIX	mProj;		// �ˉe�ϊ��s��(�]�u�s��)
	XMMATRIX	mTex;		// �e�N�X�`���ϊ��s��(�]�u�s��)
};

// ������
HRESULT CPolygon::Init()
{
	BackBuffer* buffer = BackBuffer::GetBuffer();

	HRESULT hr = S_OK;

	// �V�F�[�_������
	static const D3D11_INPUT_ELEMENT_DESC vertex_desc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	
	//���_���C�A�E�g�쐬
	hr = buffer->GetDevice()->CreateInputLayout(
		vertex_desc,				// ���C�A�E�g�ݒ�
		ARRAYSIZE(vertex_desc),		// �z��T�C�Y
		buffer->GetVertexShader(VERTEX2D)->GetData(),	// ���C�A�E�g�Ɗ֘A�t���钸�_�V�F�[�_�̃f�[�^
		buffer->GetVertexShader(VERTEX2D)->GetSize(),	// ���C�A�E�g�Ɗ֘A�t���钸�_�V�F�[�_�̃T�C�Y
		&m_pInputLayout);			// �쐬���ꂽ���_���C�A�E�g�̊i�[��

	m_pConstantBuffer = new ConstantBuffer;
	m_pConstantBuffer->Create(sizeof(SHADER_GLOBAL));

	// �ϊ��s�񏉊���
	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mView, XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	XMStoreFloat4x4(&m_mProj,
		XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 100.0f));
	XMStoreFloat4x4(&m_mTex, XMMatrixIdentity());
	m_mTex._44 = 0.0f;

	m_vPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_vColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_bInvalidate = false;

	m_vPosTexFrame = XMFLOAT2(0.0f, 0.0f);
	m_vSizeTexFrame = XMFLOAT2(1.0f, 1.0f);

	// ���_���̍쐬
	hr = MakeVertex(buffer->GetDevice());

	return hr;
}

// �I������
void CPolygon::Fin(void)
{
	// ���_�o�b�t�@�̉��
	SAFE_RELEASE(m_pVertexBuffer);
	// �萔�o�b�t�@�̉��
	SAFE_DELETE(m_pConstantBuffer);
	// ���_�t�H�[�}�b�g���
	SAFE_RELEASE(m_pInputLayout);
}

// �`��
void CPolygon::Draw()
{
	ID3D11DeviceContext* pDeviceContext = BackBuffer::GetBuffer()->GetDeviceContext();

	BackBuffer::GetBuffer()->SetUpContext(VERTEX2D,PIXEL2D, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �g�k
	XMMATRIX mWorld = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	// ��]
	mWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vAngle.x),
		XMConvertToRadians(m_vAngle.y), XMConvertToRadians(m_vAngle.z));
	// �ړ�
	mWorld *= XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	// ���[���h �}�g���b�N�X�ɐݒ�
	XMStoreFloat4x4(&m_mWorld, mWorld);

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

	BackBuffer::GetBuffer()->SetTexture(m_pTexture);

	SHADER_GLOBAL cb;
	cb.mProj = XMMatrixTranspose(XMLoadFloat4x4(&m_mProj));
	cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&m_mView));
	cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_mWorld));
	cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(&m_mTex));
	m_pConstantBuffer->Update(&cb);
	m_pConstantBuffer->SetVertexShader();
	m_pConstantBuffer->SetPixelShader();

	// �|���S���̕`��
	pDeviceContext->Draw(NUM_VERTEX, 0);
}

// ���_�̍쐬
HRESULT CPolygon::MakeVertex(ID3D11Device* pDevice)
{
	// ���_���W�̐ݒ�
	m_vertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	m_vertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	m_vertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	m_vertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);

	// �g�U���ˌ��̐ݒ�
	m_vertexWk[0].diffuse = m_vColor;
	m_vertexWk[1].diffuse = m_vColor;
	m_vertexWk[2].diffuse = m_vColor;
	m_vertexWk[3].diffuse = m_vColor;

	// �e�N�X�`�����W�̐ݒ�
	m_vertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	m_vertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	m_vertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	m_vertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(m_vertexWk);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &m_vertexWk[0];

	HRESULT hr = pDevice->CreateBuffer(&vbd, &initData, &m_pVertexBuffer);

	return hr;
}

// ���_���W�̐ݒ�
void CPolygon::SetVertex(void)
{
	if (m_bInvalidate) {
		//���_�o�b�t�@�̒��g�𖄂߂�
		ID3D11DeviceContext* pDeviceContext = BackBuffer::GetBuffer()->GetDeviceContext();
		HRESULT hr = S_OK;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		hr = pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (SUCCEEDED(hr)) {
			// �g�U���ˌ��̐ݒ�
			m_vertexWk[0].diffuse = m_vColor;
			m_vertexWk[1].diffuse = m_vColor;
			m_vertexWk[2].diffuse = m_vColor;
			m_vertexWk[3].diffuse = m_vColor;
			// ���_�f�[�^���㏑��
			memcpy_s(msr.pData, sizeof(m_vertexWk), m_vertexWk, sizeof(m_vertexWk));
			// ���_�f�[�^���A�����b�N����
			pDeviceContext->Unmap(m_pVertexBuffer, 0);
			// �t���O���N���A
			m_bInvalidate = false;
		}
	}
}

// �e�N�X�`���̐ݒ�
void CPolygon::SetTexture(ID3D11ShaderResourceView* pTexture)
{
	m_pTexture = pTexture;
	m_mTex._44 = (m_pTexture) ? 1.0f : 0.0f;
}

// �\�����W�̐ݒ�
void CPolygon::SetPos(float fX, float fY)
{
	m_vPos.x = fX;
	m_vPos.y = fY;
}

// �\���T�C�Y�̐ݒ�
void CPolygon::SetSize(float fScaleX, float fScaleY)
{
	m_vScale.x = fScaleX;
	m_vScale.y = fScaleY;
}

// �\���p�x�̐ݒ�(�P��:�x)
void CPolygon::SetAngle(float fAngle)
{
	m_vAngle.z = fAngle;
}

// ����e�N�X�`�����W�̐ݒ� (0.0��fU��1.0, 0.0��fV��1.0)
void CPolygon::SetUV(float fU, float fV)
{
	m_vPosTexFrame.x = fU;
	m_vPosTexFrame.y = fV;
}

// �e�N�X�`���t���[���T�C�Y�̐ݒ� (0.0��fWidth��1.0, 0.0��fHeight��1.0)
void CPolygon::SetFrameSize(float fWidth, float fHeight)
{
	m_vSizeTexFrame.x = fWidth;
	m_vSizeTexFrame.y = fHeight;
}

// ���_�J���[�̐ݒ�
void CPolygon::SetColor(float fRed, float fGreen, float fBlue)
{
	SetColor(XMFLOAT3(fRed, fGreen, fBlue));
}
void CPolygon::SetColor(XMFLOAT3 vColor)
{
	if (vColor.x != m_vColor.x || vColor.y != m_vColor.y || vColor.z != m_vColor.z) {
		m_vColor.x = vColor.x;
		m_vColor.y = vColor.y;
		m_vColor.z = vColor.z;
		m_bInvalidate = true;
	}
}

// �s�����x�̐ݒ�
void CPolygon::SetAlpha(float fAlpha)
{
	if (fAlpha != m_vColor.w) {
		m_vColor.w = fAlpha;
		m_bInvalidate = true;
	}
}

// ���_�J���[�̐ݒ�
void CPolygon::SetColor(float fRed, float fGreen, float fBlue, float fAlpha)
{
	SetColor(XMFLOAT4(fRed, fGreen, fBlue, fAlpha));
}
void CPolygon::SetColor(XMFLOAT4 vColor)
{
	if (vColor.x != m_vColor.x || vColor.y != m_vColor.y || vColor.z != m_vColor.z || vColor.w != m_vColor.w) {
		m_vColor = vColor;
		m_bInvalidate = true;
	}
}
