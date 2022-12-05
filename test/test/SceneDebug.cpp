//=============================================================================
//
// �V�[���f�o�b�O �N���X [SceneDebug.cpp]
// Author : Totsuka Kensuke
//
//=============================================================================
#include "sceneDebug.h"
#include "Player.h"
#include "grid.h"
#include "GUI.h"
#include "UI.h"
#include "PlayIcon.h"
#include "CollisionList.h"
#include "GlobalData.h"
#include "RotIcon.h"
#include "JumpIcon.h"


void CDebug::Init()
{
	ID3D11Device* pDevice = BackBuffer::GetBuffer()->GetDevice();


	//�f�[�^���[�h
	DataLoad(m_NameList);

	//�ʃI�u�W�F�N�g�ݒ�----------------------------------
	Create<Grid>("Grid");
	Create<CPlayer>("Player");

	GetComponent<Grid>("Grid")->Init();
	GetComponent<CPlayer>("Player")->Init();
	//-------------------------------------------------------

	//UI�ݒ�-------------------------------------------------
	if (Create<CPlayIcon>("PlayIcon"))
		GetComponent<CPlayIcon>("PlayIcon")->Init();
	if (Create<CRotIcon>("LRotIcon"))
	{
		GetComponent<CRotIcon>("LRotIcon")->Init(XMFLOAT2(-500.0f, -280.0f), 1);
		GetComponent<CRotIcon>("LRotIcon")->SetPlayer(GetComponent<CPlayer>("Player"));
	}
	if (Create<CRotIcon>("RrotIcon"))
	{
		GetComponent<CRotIcon>("RrotIcon")->Init(XMFLOAT2(-350.0f, -280.0f), 0);
		GetComponent<CRotIcon>("RrotIcon")->SetPlayer(GetComponent<CPlayer>("Player"));
	}
	if (Create<CJumpIcon>("JumpIcon"))
	{
		GetComponent<CJumpIcon>("JumpIcon")->Init(XMFLOAT2(-200.0f, -280.0f));
		GetComponent<CJumpIcon>("JumpIcon")->SetPlayer(GetComponent<CPlayer>("Player"));
	}
	//-------------------------------------------------------

}

void CDebug::Uninit()
{
	//�I�����ɃZ�[�u�𑣂�-------------------------------------------------------------
	if (MessageBox(NULL, _T("�Z�[�u���܂����H"), _T("�g�c�J����"), MB_YESNO) == IDYES)
	{
		for (auto list : m_NameList)
		{
			GetComponent<CMesh>(list.data())->SaveFile();
		}
		DataSave(m_NameList);

		GetComponent<CPlayer>("Player")->SaveFile();

		CCamera::Get()->Uninit();
	}
	//-------------------------------------------------------------------------------

	GetComponent<CPlayer>("Player")->Uninit();

	GetComponent<CPlayIcon>("PlayIcon")->Fin();
	GetComponent<CRotIcon>("LRotIcon")->Fin();
	GetComponent<CRotIcon>("RrotIcon")->Fin();
	GetComponent<CJumpIcon>("JumpIcon")->Fin();

}

void CDebug::Update()
{

	//�I�u�W�F�N�g�𐶐�-------------------------------------------------------------------------
	static CREATE_OBJECT obj;

	if (!GlobalData::Get()->GetStartFlag())
	{
		obj = GUI::Get()->DebugDisplay();
	}
	if (obj.bCreate)
	{
		m_NameList.push_back(obj.cName);
		bool bflag = false;

		switch (obj.type)
		{
		case BOX:
			bflag = Create<Box>(obj.cName);
			if (!bflag)
				break;
			GetComponent<Box>(obj.cName)->SetName(obj.cName);
			GetComponent<Box>(obj.cName)->Init(XMFLOAT3(1.0f, 1.0f, 1.0f));
			break;
		case SPHERE:
			break;
		case FBX:
			bflag = Create<Model>(obj.cName);
			if (!bflag)
				break;
			GetComponent<Model>(obj.cName)->SetName(obj.cName);
			GetComponent<Model>(obj.cName)->SetFileName(obj.cPath);
			GetComponent<Model>(obj.cName)->Init();
			break;
		}
	
		if(!bflag)
			MessageBox(NULL, _T("�������O�͎g���Ȃ��y�R�ˁE�E�E"), _T("���߂�y�R"), MB_OK);
	
	
	}

	//----------------------------------------------------------------------------------------------

	for (auto list : m_NameList)
	{
		switch (GetComponent<ObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<Box>(list.data())->Update(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<Model>(list.data())->Update(); break;
		}
	}

	GetComponent<CPlayer>("Player")->Update();

	GetComponent<CPlayIcon>("PlayIcon")->Update();
	GetComponent<CRotIcon>("LRotIcon")->Update();
	GetComponent<CRotIcon>("RrotIcon")->Update();
	GetComponent<CJumpIcon>("JumpIcon")->Update();

	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	{
		if (GetAsyncKeyState('S') & 0x8000)
		{
			MessageBox(NULL, _T("�Z�[�u���܂���"), _T("�g�c�J����"), MB_OK);

			for (auto list : m_NameList)
			{
				GetComponent<Box>(list.data())->SaveFile();
			}
			DataSave(m_NameList);

			GetComponent<CPlayer>("Player")->SaveFile();
		}
	}
}

void CDebug::Draw()
{
	BackBuffer* buffer  = BackBuffer::GetBuffer();


	GetComponent<Grid>("Grid")->Draw();
	GetComponent<CPlayer>("Player")->Draw();

	for (auto list : m_NameList)
	{
		switch (GetComponent<ObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<Box>(list.data())->Draw(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<Model>(list.data())->Draw(); break;
		}
	}

	buffer->SetZBuffer(false);
	buffer->SetBlendState(BS_ALPHABLEND);
	GetComponent<CPlayIcon>("PlayIcon")->Draw();
	GetComponent<CRotIcon>("LRotIcon")->Draw();
	GetComponent<CRotIcon>("RrotIcon")->Draw();
	GetComponent<CJumpIcon>("JumpIcon")->Draw();
	buffer->SetBlendState();
	buffer->SetZBuffer(true);
}