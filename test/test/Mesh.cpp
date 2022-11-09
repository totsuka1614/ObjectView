//=============================================================================
//
// ���b�V�� �N���X [Mesh.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================

#include "Mesh.h"
#include "backbuffer.h"
#include "Camera.h"

//�萔��`
#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.1f,0.1f,0.1f,1.0f)
#define M_AMBIENT		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

HRESULT CMesh::Init()
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

	//���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(VERTEX_3D) * vertex.second.size();	// �o�b�t�@�̃T�C�Y
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// �g�p���@
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND�ݒ�
	buffer_desc.CPUAccessFlags = 0;						// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
	buffer_desc.MiscFlags = 0;							// ���\�[�X�I�v�V�����̃t���O
	buffer_desc.StructureByteStride = 0;				// �\���̂̃T�C�Y

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = &vertex.second[0];	// �o�b�t�@�̒��g�̐ݒ�
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
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = (UINT)sizeof(UINT) * index.second.size();	// �o�b�t�@�̃T�C�Y
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// �g�p���@
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND�ݒ�
	buffer_desc.CPUAccessFlags = 0;										// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
	buffer_desc.MiscFlags = 0;											// ���\�[�X�I�v�V�����̃t���O
	buffer_desc.StructureByteStride = 0;								// �\���̂̃T�C�Y

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = &index.second[0];							// �o�b�t�@�̒��g�̐ݒ�
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


}