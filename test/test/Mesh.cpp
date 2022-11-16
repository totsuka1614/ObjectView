//=============================================================================
//
// ���b�V�� �N���X [Mesh.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================

#include "Mesh.h"
#include "backbuffer.h"
#include "Camera.h"
#include "light.h"

//�萔��`
#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,0.7f)
#define M_SPECULAR		XMFLOAT4(0.1f,0.1f,0.1f,1.0f)
#define M_AMBIENT		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

HRESULT CMesh::Init(VERTEX_3D Vertex[], int nVertex, int* Index, int nIndex)
{
	

	BackBuffer* buffer = BackBuffer::GetBuffer();
	ID3D11Device* device = buffer->GetDevice();

	m_pConstantBuffer[0]->Create(sizeof(CONSTANT_BUFFER));
	m_pConstantBuffer[1]->Create(sizeof(CONSTANT_BUFFER2));

	// �}�e���A���̏����ݒ�
	m_Material.Diffuse = M_DIFFUSE;
	m_Material.Ambient = M_AMBIENT;
	m_Material.Specular = M_SPECULAR;
	m_Material.Power = 0.0f;
	m_Material.Emissive = M_EMISSIVE;

	m_nIndex = nIndex;

	D3D11_INPUT_ELEMENT_DESC vertex_desc[]{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	HRESULT hr;

	//���_���C�A�E�g�쐬
	hr = device->CreateInputLayout(
		vertex_desc,				// ���C�A�E�g�ݒ�
		ARRAYSIZE(vertex_desc),		// �z��T�C�Y
		buffer->GetVertexShader()->GetData(),	// ���C�A�E�g�Ɗ֘A�t���钸�_�V�F�[�_�̃f�[�^
		buffer->GetVertexShader()->GetSize(),	// ���C�A�E�g�Ɗ֘A�t���钸�_�V�F�[�_�̃T�C�Y
		&m_InputLayout);			// �쐬���ꂽ���_���C�A�E�g�̊i�[��

	if (FAILED(hr))
		return hr;

	//���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(VERTEX_3D) * nVertex;	// �o�b�t�@�̃T�C�Y
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// �g�p���@
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND�ݒ�
	buffer_desc.CPUAccessFlags = 0;						// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
	buffer_desc.MiscFlags = 0;							// ���\�[�X�I�v�V�����̃t���O
	buffer_desc.StructureByteStride = 0;				// �\���̂̃T�C�Y

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = Vertex;	// �o�b�t�@�̒��g�̐ݒ�
	sub_resource.SysMemPitch = 0;			// texture�f�[�^���g�p����ۂɎg�p���郁���o
	sub_resource.SysMemSlicePitch = 0;		// texture�f�[�^���g�p����ۂɎg�p���郁���o

	// �o�b�t�@�쐬
	if (FAILED(device->CreateBuffer(
		&buffer_desc,								// �o�b�t�@���
		&sub_resource,								// ���\�[�X���
		&m_VertexBuffers)))	// �쐬���ꂽ�o�b�t�@�̊i�[��
	{
		return false;
	}

	//���_�o�b�t�@�쐬
	buffer_desc.ByteWidth = (UINT)sizeof(UINT) * nIndex;	// �o�b�t�@�̃T�C�Y
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// �g�p���@
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND�ݒ�
	buffer_desc.CPUAccessFlags = 0;										// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
	buffer_desc.MiscFlags = 0;											// ���\�[�X�I�v�V�����̃t���O
	buffer_desc.StructureByteStride = 0;								// �\���̂̃T�C�Y

	sub_resource.pSysMem = Index;							// �o�b�t�@�̒��g�̐ݒ�
	sub_resource.SysMemPitch = 0;										// texture�f�[�^���g�p����ۂɎg�p���郁���o
	sub_resource.SysMemSlicePitch = 0;									// texture�f�[�^���g�p����ۂɎg�p���郁���o

	// �o�b�t�@�쐬
	if (FAILED(device->CreateBuffer(
		&buffer_desc,						// �o�b�t�@���
		&sub_resource,						// ���\�[�X���
		&m_IndexBuffers)))		// �쐬���ꂽ�o�b�t�@�̊i�[��
	{
		return false;
	}

	return hr;
}

void CMesh::Update()
{
	// ���[���h�}�g���N�X�ݒ�
	XMMATRIX translate = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMMATRIX rotate_x = XMMatrixRotationX(XMConvertToRadians(m_vDegree.x));
	XMMATRIX rotate_y = XMMatrixRotationY(XMConvertToRadians(m_vDegree.y));
	XMMATRIX rotate_z = XMMatrixRotationZ(XMConvertToRadians(m_vDegree.z));
	XMMATRIX scale_mat = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	m_mtxWorld = scale_mat * rotate_x * rotate_y * rotate_z * translate;

}

void CMesh::Draw(XMMATRIX& mtxWorld, VSShaderType vstype, PSShaderType pstype)
{
	BackBuffer *buffer = BackBuffer::GetBuffer();
	buffer->SetUpContext(vstype,pstype);
	UINT strides = sizeof(VERTEX_3D);
	UINT offsets = 0;

	//�R���X�^���g�o�b�t�@
	CONSTANT_BUFFER cb;
	//�J�������
	CCamera* pCamera = CCamera::Get();

	// ���[���h�}�g���N�X���R���X�^���g�o�b�t�@�ɐݒ�
	cb.mWVP = XMMatrixTranspose(mtxWorld * XMLoadFloat4x4(&pCamera->GetViewMatrix()) * XMLoadFloat4x4(&pCamera->GetProjMatrix()));
	cb.mW = XMMatrixTranspose(mtxWorld);
	XMVECTOR light = DirectX::XMVector3Normalize(DirectX::XMVectorSet(CLight::Get()->GetDir().x, CLight::Get()->GetDir().y, CLight::Get()->GetDir().z, 0.0f));
	XMStoreFloat4(&cb.fLightVector, light);
	// �R���X�^���g�o�b�t�@�X�V
	m_pConstantBuffer[0]->Update(&cb);

	CONSTANT_BUFFER2 cb2;
	CLight* pLight = CLight::Get();
	cb2.vEye = XMLoadFloat3(&pCamera->GetTransform());
	cb2.vLightDir = XMLoadFloat3(&pLight->GetDir());
	cb2.vLa = XMLoadFloat4(&pLight->GetAmbient());
	cb2.vLd = XMLoadFloat4(&pLight->GetDiffuse());
	cb2.vLs = XMLoadFloat4(&pLight->GetSpecular());
	cb2.vDiffuse = XMLoadFloat4(&m_Material.Diffuse);
	cb2.vAmbient = XMVectorSet(m_Material.Ambient.x, m_Material.Ambient.y, m_Material.Ambient.z, 0.f);
	cb2.vSpecular = XMVectorSet(m_Material.Specular.x, m_Material.Specular.y, m_Material.Specular.z, m_Material.Power);
	cb2.vEmissive = XMLoadFloat4(&m_Material.Emissive);
	m_pConstantBuffer[1]->Update(&cb2);

	// �C���f�b�N�X�o�b�t�@�̐� = �}�e���A���̐��������b�V����`�悷��
	// IA(InputAssemblerStage)�ɓ��̓��C�A�E�g��ݒ肷��
	buffer->GetDeviceContext()->IASetInputLayout(m_InputLayout);
	// IA�ɐݒ肷�钸�_�o�b�t�@�̎w��
	buffer->GetDeviceContext()->IASetVertexBuffers(
		0,						// �o�b�t�@���M�̃X���b�g�ԍ�
		1,						// �o�b�t�@�̐�
		&m_VertexBuffers,		// ���_�o�b�t�@
		&strides,				// �o�b�t�@�Ɏg�p���Ă���\���̂̃T�C�Y
		&offsets);				// �J�n�I�t�Z�b�g

	buffer->GetDeviceContext()->IASetIndexBuffer(
		m_IndexBuffers,
		DXGI_FORMAT_R32_UINT,
		0);

	// �R���e�L�X�g�ɃR���X�^���g�o�b�t�@��ݒ�
	m_pConstantBuffer[0]->SetVertexShader();
	m_pConstantBuffer[1]->SetPixelShader();

	// �`��
	buffer->GetDeviceContext()->DrawIndexed(
		m_nIndex,		// ���_��
		0,						// �I�t�Z�b�g
		0);						// �J�n���_�̃C���f�b�N�X
	
}