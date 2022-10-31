//=============================================================================
//
// ���f�� �N���X [Model.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "model.h"
#include "GUI.h"
#include <string.h>
#include <string>
#include "Input.h"

#define MODEL_NAME "data/model/chara.fbx"

void Model::Init(void)
{
	//FBX�t�@�C���̓ǂݍ���
	m_ModelData = new FBXFile;

	m_ModelData->Load(MODEL_NAME);

	strcpy(m_cFileName, MODEL_NAME);
	
	m_mtxWorld = XMMatrixIdentity();
	m_PStype = PIXEL;
	m_vPos		= XMFLOAT3(0.0f, 10.0f, 0.0f);
	m_vDegree	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale	= XMFLOAT3(1.0f, 1.0f, 1.0f);
}

void Model::Uninit(void)
{
	delete m_ModelData;
}

void Model::Update(void)
{
	//if(CInput::GetKeyTrigger(VK_A))
	//{
	//	//--- �u�t�@�C�����J���v�_�C�A���O�̕\��
	////�_�C�A���O�̏����ݒ�
	//	char fileName[MAX_PATH] = "";	//�������Ŋk�ɂ��Ă����Ȃ��ƁA�ŏ��̗����Ŏw�肳��邽�߁A�J���Ȃ�
	//	OPENFILENAME ofn;
	//	ZeroMemory(&ofn, sizeof(ofn));
	//	ofn.lStructSize = sizeof(ofn);
	//	ofn.hwndOwner = GetActiveWindow();
	//	ofn.lpstrFile = fileName;			//�J���őI�������t�@�C���p�X���i�[
	//	ofn.nMaxFile = sizeof(fileName);
	//	//�_�C�A���O�E���́A�t�@�C���g���q�̃t�B���^�[�ݒ�
	//	//[�\����\0�g���q\0]�̑g�ݍ��킹�A\0�ŋ�؂�
	//	ofn.lpstrFilter = _T("ALL Files\0*.*\0.fbx\0*fbx\0");
	//	ofn.nFilterIndex = 2;				//�ŏ��Ɏw�肵�Ă���t�B���^�[
	//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//	//�_�C�A���O��\��
	//	//���t�@�C���w�肪�ł�����TRUE��������
	//	if (TRUE == GetOpenFileName(&ofn))
	//	{
	//		std::string file_path = fileName;
	//		int len = strlen(fileName);
	//		int path_start_point = 1;

	//		for (int i = len - 1; i >= 0; i--)
	//		{
	//			if (fileName[i] == '\\')
	//			{
	//				path_start_point = i;
	//				break;
	//			}
	//		}
	//		file_path = file_path.substr(path_start_point + 1);

	//		}
	//}

	//m_vDegree.y += 1.0f;

	// ���[���h�}�g���N�X�ݒ�
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	DirectX::XMMATRIX rotate_x = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_vDegree.x));
	DirectX::XMMATRIX rotate_y = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_vDegree.y));
	DirectX::XMMATRIX rotate_z = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_vDegree.z));
	DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	m_mtxWorld = scale_mat * rotate_x * rotate_y * rotate_z * translate;

	//m_vDegree.y += 0.5f;

	GUI::Get()->Create(*this);
}

void Model::Draw(void)
{
	if (m_ModelData)
	{
		m_ModelData->Draw(m_mtxWorld, m_PStype);
	}
}