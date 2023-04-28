/******************************************************************************
* 
* @file      SceneDebug.cpp
* @brief     �f�o�b�O�V�[��
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention ���݂��̃V�[�������C��
* 
******************************************************************************/
#include "sceneDebug.h"
#include "Player.h"
#include "grid.h"
#include "GUI.h"
#include "UI.h"
#include "input.h"
#include "PlayIcon.h"
#include "CollisionList.h"
#include "GlobalData.h"
#include "RotIcon.h"
#include "JumpIcon.h"
#include "StopIcon.h"
#include "SceneManager.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "Filter.h"
#include "light.h"

/******************************************************************************
* 
* @brief      Init
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ������
* @attention  
******************************************************************************/
void CDebug::Init()
{
	//�f�o�C�X�擾
	ID3D11Device* pDevice = BACKBUFFER->GetDevice();

	//��ʕ���
	m_bSplit = false;	
	m_split.Init();

	//�f�[�^���[�h
	DataLoad(m_NameList);

	//RenderTarget------------------------------------------

	//���C���L�����o�X
	CRenderTarget* render = new CRenderTarget;
	render->Create(DXGI_FORMAT_B8G8R8A8_UNORM);
	Entry<CRenderTarget>("Canvas", render);	

	//�V���h�E�L�����o�X
	CRenderTarget* render2 = new CRenderTarget;
	render2->Create(DXGI_FORMAT_R32_FLOAT);			
	Entry<CRenderTarget>("ShadowRender", render2);	

	//------------------------------------------------------

	//DepthStencil------------------------------------------

	//�V���h�E�X�e���V��
	CDepthStencil* depth2 = new CDepthStencil;
	depth2->Create(DXGI_FORMAT_D24_UNORM_S8_UINT);  
	Entry<CDepthStencil>("ShadowDepth", depth2);

	//------------------------------------------------------

	//�ʃI�u�W�F�N�g�ݒ�----------------------------------
	//���X�g�ɂ͂Ȃ��I�u�W�F�N�g���ʂɐݒ�

	//�O���b�h����
	Create<CGrid>("Grid");
	GetComponent<CGrid>("Grid")->Init();
	//�v���C���[����
	Create<CPlayer>("Player");
	GetComponent<CPlayer>("Player")->Init();

	//-------------------------------------------------------

	//UI�ݒ�-------------------------------------------------

	//�v���C�A�C�R������
	Create<CPlayIcon>("PlayIcon");
	GetComponent<CPlayIcon>("PlayIcon")->Init();

	//��]�A�C�R��(Left)����
	Create<CRotIcon>("LRotIcon");
	GetComponent<CRotIcon>("LRotIcon")->Init(XMFLOAT2(-500.0f, -280.0f), 1);	
	GetComponent<CRotIcon>("LRotIcon")->SetPlayer(GetComponent<CPlayer>("Player"));

	//��]�A�C�R��(Right)����
	Create<CRotIcon>("RrotIcon");
	GetComponent<CRotIcon>("RrotIcon")->Init(XMFLOAT2(-350.0f, -280.0f), 0);
	GetComponent<CRotIcon>("RrotIcon")->SetPlayer(GetComponent<CPlayer>("Player")); 

	//�W�����v�A�C�R������
	Create<CJumpIcon>("JumpIcon");
	GetComponent<CJumpIcon>("JumpIcon")->Init(XMFLOAT2(-200.0f, -280.0f));
	GetComponent<CJumpIcon>("JumpIcon")->SetPlayer(GetComponent<CPlayer>("Player"));

	//��~�A�C�R������
	Create<CStopIcon>("StopIcon");
	GetComponent<CStopIcon>("StopIcon")->Init(XMFLOAT2(-50.0f, -280.0f));
	GetComponent<CStopIcon>("StopIcon")->SetPlayer(GetComponent<CPlayer>("Player"));
	//-------------------------------------------------------------------------------------------------------

	//�t�B���^�[����--------------------------------------------------------------
	Create<CFilter>("Filter");
	GetComponent<CFilter>("Filter")->Init();
	GetComponent<CFilter>("Filter")->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	//----------------------------------------------------------------------------
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �I������
* @attention  
******************************************************************************/
void CDebug::Uninit()
{
	//�I�����ɃZ�[�u�𑣂�-------------------------------------------------------------
	if (MessageBox(NULL, _T("�Z�[�u���܂����H"), _T(""), MB_YESNO) == IDYES)
	{
		for (auto list : m_NameList)	//�I�u�W�F�N�g�̐����J��Ԃ�
		{
			switch (GetComponent<CObjectBase>(list.data())->GetType())	//�I�u�W�F�N�g�^�C�v���ƂɃZ�[�u
			{
			case BOX:	//�{�b�N�X
				GetComponent<CBox>(list.data())->SaveFile(); break;
			case SPHERE://�X�t�B�A(������)
				break;
			case FBX:	//FBX���f��
				GetComponent<CModel>(list.data())->SaveFile();
				GetComponent<CModel>(list.data())->Uninit(); break;
			}
		}
		
		//�I�u�W�F�N�g���X�g�Z�[�u
		DataSave(m_NameList);
		m_NameList.clear();	

		//�v���C���[�Z�[�u
		GetComponent<CPlayer>("Player")->SaveFile();

		//�J�����I������
		CCamera::Get()->Uninit();
	}
	//-------------------------------------------------------------------------------

	//�v���C���[�I������
	GetComponent<CPlayer>("Player")->Uninit();
	//�v���C�A�C�R���I������
	GetComponent<CPlayIcon>("PlayIcon")->Fin();
	//��]�A�C�R��(Left)�I������
	GetComponent<CRotIcon>("LRotIcon")->Fin();
	//��]�A�C�R��(Right)�I������
	GetComponent<CRotIcon>("RrotIcon")->Fin();
	//�W�����v�A�C�R���I������
	GetComponent<CJumpIcon>("JumpIcon")->Fin();
	//��~�A�C�R���I������
	GetComponent<CStopIcon>("StopIcon")->Fin();
	//�t�B���^�[�I������
	GetComponent<CPolygon>("Filter")->Fin();
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �X�V����
* @attention  
******************************************************************************/
void CDebug::Update()
{
	//�I�u�W�F�N�g�𐶐�-------------------------------------------------------------------------
	/*
	�Q�[�����Ń��A���^�C���ŃI�u�W�F�N�g�𐶐�����@�\
	obj.bCreate��true�ɂȂ������A�������n�߂�
	*/

	//�쐬�I�u�W�F�N�g�̐錾
	static CREATE_OBJECT obj;

	if (!GLOBALDATA->GetStartFlag())	//�Q�[���J�n����Ă��Ȃ��ꍇ
		obj = IMGUI->DebugDisplay();	//�f�o�b�O��\��(���������I�u�W�F�N�g��Ԃ��B�Ȃ����bCreate��false�ŕԂ�)
	
	if (obj.bCreate)		//�I�u�W�F�N�g���������ꂽ�ꍇ
	{
		//�������O���g���Ă��邩�̔���
		bool bNameFlag = false;

		switch (obj.type)	//�I�u�W�F�N�g�^�C�v���Ƃɐ���
		{
		case BOX:			//�{�b�N�X
			bNameFlag = Create<CBox>(obj.cName);		//����(�����������O���g���Ă����玸�s(false))
	
			if (!bNameFlag)	//�������O���g���Ă����ꍇ
				break;
			//������
			GetComponent<CBox>(obj.cName)->SetName(obj.cName);
			GetComponent<CBox>(obj.cName)->Init(XMFLOAT3(1.0f, 1.0f, 1.0f));

			//���X�g�ɓo�^
			m_NameList.push_back(obj.cName);
			break;
		case SPHERE:		//������
			break;
		case FBX:			//FBX���f��
			bNameFlag = Create<CModel>(obj.cName);	//����(�����������O���g���Ă����玸�s(false))
			if (!bNameFlag)
				break;
			
			//������
			GetComponent<CModel>(obj.cName)->SetName(obj.cName);
			GetComponent<CModel>(obj.cName)->SetFileName(obj.cPath);
			GetComponent<CModel>(obj.cName)->Init();

			//���X�g�ɓo�^
			m_NameList.push_back(obj.cName);
			break;
		}
	
		//�|�b�v�A�b�v�\��
		if(!bNameFlag)
			MessageBox(NULL, _T("�������O�͎g���܂���"), _T("error"), MB_OK);
	
	
	}

	//----------------------------------------------------------------------------------------------

	//�X�V-----------------------------------------------------------------------------
	for (auto list : m_NameList)	//�I�u�W�F�N�g�̐����J��Ԃ�
	{
		switch (GetComponent<CObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<CBox>(list.data())->Update(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<CModel>(list.data())->Update(); break;
		}
	}

	//�X�V����
	GetComponent<CPlayer>("Player")->Update();
	GetComponent<CGrid>("Grid")->Update();
	GetComponent<CPlayIcon>("PlayIcon")->Update();
	GetComponent<CRotIcon>("LRotIcon")->Update();
	GetComponent<CRotIcon>("RrotIcon")->Update();
	GetComponent<CJumpIcon>("JumpIcon")->Update();
	GetComponent<CStopIcon>("StopIcon")->Update();
	GetComponent<CFilter>("Filter")->Update();
	//---------------------------------------------------------------------------------------------

	//��ʕ����@�\(Control + 1)
	if (CInput::GetKeyPress(VK_LCONTROL))
		if(CInput::GetKeyTrigger(VK_1))
			m_bSplit = !m_bSplit;

	//�r���Z�[�u�@�\(Control + S)
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	{
		if (GetAsyncKeyState('S') & 0x8000)
		{
			MessageBox(NULL, _T("�Z�[�u���܂���"), _T(""), MB_OK);

			for (auto list : m_NameList)//�I�u�W�F�N�g�̐����J��Ԃ�
			{
				switch (GetComponent<CObjectBase>(list.data())->GetType())
				{
				case BOX:
					GetComponent<CBox>(list.data())->SaveFile(); break;
				case SPHERE:
					break;
				case FBX:
					GetComponent<CModel>(list.data())->SaveFile();
					GetComponent<CModel>(list.data())->Uninit(); break;
				}
			}
			DataSave(m_NameList);

			GetComponent<CPlayer>("Player")->SaveFile();
		}
	}
}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �`�揈��
* @attention  
******************************************************************************/
void CDebug::Draw()
{
	//�o�b�t�@�Q�b�g
	CBackBuffer* buffer = BACKBUFFER;
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//RenderingTarget�̐ݒ�	 - ToDo : �����Ɗȑf�ɂ���
	CRenderTarget* pRtv = GetComponent<CRenderTarget>("Canvas");
	ID3D11RenderTargetView* pView = pRtv->GetView();

	//�V���h�E�L�����o�X�ɕύX
	CRenderTarget* pShadowRtv = GetComponent<CRenderTarget>("ShadowRender");
	CDepthStencil* pShadowDsv = GetComponent<CDepthStencil>("ShadowDepth");

	//�V���h�E�L�����o�X���Z�b�g
	pView = pShadowRtv->GetView();
	buffer->GetDeviceContext()->ClearRenderTargetView(pView, color);
	buffer->GetDeviceContext()->ClearDepthStencilView(pShadowDsv->GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	buffer->GetDeviceContext()->OMSetRenderTargets(1, &pView, pShadowDsv->GetView());

	//�J�����𑾗z�̈ʒu��
	CCamera::Get()->SetSun();

	//�`��-----------------------------
	buffer->SetUpViewPort();
	//DrawDepthShadow();
	//---------------------------------

	//�J���������̈ʒu��
	CCamera::Set();

	//���̃L�����o�X�ɕύX���Z�b�g
	pView = pRtv->GetView();
	buffer->GetDeviceContext()->ClearRenderTargetView(pView, color);
	buffer->GetDeviceContext()->ClearDepthStencilView(buffer->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	buffer->GetDeviceContext()->OMSetRenderTargets(1, &pView, buffer->GetDepthStencilView());

	//�X�J�C�{�b�N�X�̕`��
	CCamera::Get()->Sky();

	//�`��-------------------------------------------------------------------
	if (!m_bSplit)	//������ʂł͂Ȃ�
	{
		buffer->SetUpViewPort();
		//DrawShadow(pShadowRtv->GetResource());
		DrawObj();
		Draw2D();
	}				
	else            //������ʂł���
	{
		SplitDraw(pShadowRtv->GetResource());
	}
	//-------------------------------------------------------------------------

	//�^�̕`��----------------------------------------------------------------------------------
	pView = buffer->GetRenderTargetView();
	buffer->GetDeviceContext()->ClearRenderTargetView(pView, color);
	buffer->GetDeviceContext()->OMSetRenderTargets(1, &pView , buffer->GetDepthStencilView());

	GetComponent<CFilter>("Filter")->SetTexture(pRtv->GetResource());
	GetComponent<CFilter>("Filter")->Draw();
	//-------------------------------------------------------------------------------------------*/
}

/******************************************************************************
* 
* @brief      SplitDraw
* @param[in]  ShadowTex
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �����`��(�ォ��A������A�O����A���݂̃J�����̂S��)
* @attention  
******************************************************************************/
void CDebug::SplitDraw(ID3D11ShaderResourceView* ShadowTex)
{
	//�o�b�t�@�Q�b�g
	CBackBuffer* buffer = BACKBUFFER;

	//��ʃN���A
	CCamera::Get()->Clear();

	//����
	buffer->SetUpViewPort(0.0f, 0.0f, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	m_split.SetMode(UP_VIEW);
	CCamera::Set(&m_split);
	CCamera::Get()->Sky();
	DrawShadow(ShadowTex);
	
	//�E��
	buffer->SetUpViewPort(SCREEN_CENTER_X, 0.0f, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	m_split.SetMode(SIDE_VIEW);
	CCamera::Get()->Sky();
	DrawShadow(ShadowTex);

	//����
	buffer->SetUpViewPort(0.0f, SCREEN_CENTER_Y, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	m_split.SetMode(FRONT_VIEW);
	CCamera::Get()->Sky();
	DrawShadow(ShadowTex);

	//�E��
	CCamera::Set();
	buffer->SetUpViewPort(SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_CENTER_X, SCREEN_CENTER_Y);
	CCamera::Get()->Sky();
	DrawShadow(ShadowTex);

	//�r���[�|�[�g�ݒ�����ɖ߂�
	buffer->SetUpViewPort();
	//UI�`��
	Draw2D();

}

/******************************************************************************
* 
* @brief      DrawObj
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �I�u�W�F�N�g�`��
* @attention  
******************************************************************************/
void CDebug::DrawObj()
{
	//�`��
	GetComponent<CGrid>("Grid")->Draw();
	GetComponent<CPlayer>("Player")->Draw();
	
	for (auto list : m_NameList)	//�I�u�W�F�N�g�̐����J��Ԃ�
	{
		switch (GetComponent<CObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<CBox>(list.data())->Draw(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<CModel>(list.data())->Draw(); break;
		}

	}
}

/******************************************************************************
* 
* @brief      Draw2D
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       2D�I�u�W�F�N�g�`��
* @attention  Z�o�b�t�@�𖳌��ɂ��A�`���ɗL���ɖ߂�����
******************************************************************************/
void CDebug::Draw2D()
{
	//�o�b�t�@�Q�b�g
	CBackBuffer* buffer = BACKBUFFER;

	//Z�o�b�t�@����
	buffer->SetZBuffer(false);
	
	//�`��
	buffer->SetBlendState(BS_ALPHABLEND);
	GetComponent<CPlayIcon>("PlayIcon")->Draw();
	GetComponent<CRotIcon>("LRotIcon")->Draw();
	GetComponent<CRotIcon>("RrotIcon")->Draw();
	GetComponent<CJumpIcon>("JumpIcon")->Draw();
	GetComponent<CStopIcon>("StopIcon")->Draw();
	buffer->SetBlendState();
	
	//Z�o�b�t�@�L��
	buffer->SetZBuffer(true);
}

/******************************************************************************
* 
* @brief      DrawDepthShadow
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �V���h�E�}�b�v�ɏ������ݗp
* @attention  
******************************************************************************/
void CDebug::DrawDepthShadow()
{
	//�����Ⴎ����ł����A�V�F�[�_��DEPTH_WRITE�ɃZ�b�g���ĕ`�悵�Ă邾���ł�

	VSShaderType i = GetComponent<CPlayer>("Player")->GetVSType();
	PSShaderType f = GetComponent<CPlayer>("Player")->GetPSType();
	GetComponent<CPlayer>("Player")->SetVSType(DEPTHWRITEVS);
	GetComponent<CPlayer>("Player")->SetPSType(DEPTHWRITEPS);
	GetComponent<CPlayer>("Player")->Draw();
	GetComponent<CPlayer>("Player")->SetVSType(i);
	GetComponent<CPlayer>("Player")->SetPSType(f);


	for (auto list : m_NameList)//�I�u�W�F�N�g�̐����J��Ԃ�
	{
		i = GetComponent<CObjectBase>(list.data())->GetVSType();
		f = GetComponent<CObjectBase>(list.data())->GetPSType();

		GetComponent<CObjectBase>(list.data())->SetVSType(DEPTHWRITEVS);
		GetComponent<CObjectBase>(list.data())->SetPSType(DEPTHWRITEPS);

		switch (GetComponent<CObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<CBox>(list.data())->Draw(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<CModel>(list.data())->Draw(); break;
		}

		GetComponent<CObjectBase>(list.data())->SetVSType(i);
		GetComponent<CObjectBase>(list.data())->SetPSType(f);

	}
}

/******************************************************************************
* 
* @brief      DrawShadow
* @param[in]  ShadowTex
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �V���h�E�}�b�v�𗘗p�����`��
* @attention  
******************************************************************************/
void CDebug::DrawShadow(ID3D11ShaderResourceView* ShadowTex)
{
	//�V���h�E�}�b�v�ݒ�
	BACKBUFFER->SetTexture(ShadowTex, 1);
	
	//�����Ⴎ����ł����A�V�F�[�_��SHADOWVS,DEPTHSHADOWPS�ɃZ�b�g���ĕ`�悵�Ă邾���ł�
	//�`��
	VSShaderType i = GetComponent<CPlayer>("Player")->GetVSType();
	PSShaderType f = GetComponent<CPlayer>("Player")->GetPSType();
	GetComponent<CPlayer>("Player")->SetVSType(SHADOWVS);
	GetComponent<CPlayer>("Player")->SetPSType(DEPTHSHADOWPS);
	GetComponent<CGrid>("Grid")->Draw();
	GetComponent<CPlayer>("Player")->Draw();
	GetComponent<CPlayer>("Player")->SetVSType(i);
	GetComponent<CPlayer>("Player")->SetPSType(f);

	for (auto list : m_NameList)	//�I�u�W�F�N�g�̐����J��Ԃ�
	{
		i = GetComponent<CObjectBase>(list.data())->GetVSType();
		f = GetComponent<CObjectBase>(list.data())->GetPSType();

		GetComponent<CObjectBase>(list.data())->SetVSType(SHADOWVS);
		GetComponent<CObjectBase>(list.data())->SetPSType(DEPTHSHADOWPS);

		switch (GetComponent<CObjectBase>(list.data())->GetType())
		{
		case BOX:
			GetComponent<CBox>(list.data())->Draw(); break;
		case SPHERE:
			break;
		case FBX:
			GetComponent<CModel>(list.data())->Draw(); break;
		}

		GetComponent<CObjectBase>(list.data())->SetVSType(i);
		GetComponent<CObjectBase>(list.data())->SetPSType(f);

	}
}