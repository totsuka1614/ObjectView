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
#include "Polygon.h"
#include "Texture.h"

static ID3D11ShaderResourceView* texture;

void CDebug::Init()
{
	ID3D11Device* pDevice = BackBuffer::GetBuffer()->GetDevice();

	HRESULT hr;

	hr = CreateTextureFromFile(pDevice, "data/Texture/UI_ArrowRight.png", &texture);

	//�f�[�^���[�h
	DataLoad(m_NameList);

	//�ʃI�u�W�F�N�g�ݒ�----------------------------------
	Create<Grid>("Grid");
	Create<CPlayer>("Player");
	
	GetComponent<Grid>("Grid")->Init();
	GetComponent<CPlayer>("Player")->Init();
	//-------------------------------------------------------
}

void CDebug::Uninit()
{
	//�I�����ɃZ�[�u�𑣂�-------------------------------------------------------------
	if (MessageBox(NULL, _T("�Z�[�u���܂����H"), _T("�g�c�J����"), MB_YESNO) == IDYES)
	{
		for (auto list : m_NameList)
		{
			GetComponent<Box>(list.data())->SaveFile();
		}
		DataSave(m_NameList);

		GetComponent<CPlayer>("Player")->SaveFile();
	}
	//-------------------------------------------------------------------------------

	GetComponent<CPlayer>("Player")->Uninit();

}

void CDebug::Update()
{
	//�I�u�W�F�N�g�𐶐�-------------------------------------------------------------------------
	static CREATE_OBJECT obj;

	obj = GUI::Get()->DebugDisplay();

	if (obj.bCreate)
	{
		if (Create<Box>(obj.cName))
		{
			m_NameList.push_back(obj.cName);

			switch (obj.type)
			{
			case BOX:
				GetComponent<Box>(obj.cName)->SetName(obj.cName);
				GetComponent<Box>(obj.cName)->Init(XMFLOAT3(1.0f,1.0f,1.0f));
				break;
			case SPHERE:
				break;
			}
		}
		else
		{
			MessageBox(NULL, _T("�������O�͎g���Ȃ����S�˂�wwwww"), _T("error"), MB_OK);
		}
	}
	//----------------------------------------------------------------------------------------------

	for (auto list : m_NameList)
	{
		GetComponent<Box>(list.data())->Update();
	}

	GetComponent<CPlayer>("Player")->Update();

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
		GetComponent<Box>(list.data())->Draw();
	}

	buffer->SetZBuffer(false);

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	CPolygon::SetSize((float)512.0f/5.0f, (float)512.0f/5.0f);
	CPolygon::SetTexture(texture);
	CPolygon::SetPos(-500.0f, -300.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::Draw();

	buffer->SetZBuffer(true);
}