/******************************************************************************
* 
* @file      Camera.cpp
* @brief     �J�����N���X
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "Camera.h"
#include "light.h"
#include "BackBuffer.h"
#include "GUI.h"
#include <math.h>
#include <string.h>
#include "Input.h"
#include "SceneManager.h"
#include "GlobalData.h"

//*****************************************************************************
// �萔�E�}�N����`
//*****************************************************************************
#define CLEAR_COLOR (0.117647f, 0.254902f, 0.352941f, 1.0f)			//��ʃN���A�J���\
#define MOUSE_LIMIT (1.5f)											//�}�E�X����_
#define MOUSE_AXIS (0.01f)											//�}�E�X�ړ��ʌW��
#define INERTIA_IN	(0.9f)											//����IN
#define INERTIA_OUT (1.0f - INERTIA_IN)								//����OUT

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
namespace {
	const float CAM_POS_P_X = 0.0f;					// �J�����̎��_�����ʒu(X���W)
	const float CAM_POS_P_Y = 250.0f;				// �J�����̎��_�����ʒu(Y���W)
	const float CAM_POS_P_Z = -560.0f;				// �J�����̎��_�����ʒu(Z���W)
	const float CAM_POS_R_X = 0.0f;					// �J�����̒����_�����ʒu(X���W)
	const float CAM_POS_R_Y = 0.0f;					// �J�����̒����_�����ʒu(Y���W)
	const float CAM_POS_R_Z = 0.0f;					// �J�����̒����_�����ʒu(Z���W)
	const float VIEW_ANGLE = 45.0f;					// ����p
	const float VIEW_ASPECT = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;	// �A�X�y�N�g��
	const float VIEW_NEAR_Z = 1.0f;					// NearZ�l
	const float VIEW_FAR_Z = 10000.0f;				// FarZ�l

	const float TP_POS_P_X = 0.0f;		// �O�l�̃J�����̎��_�ʒu(X���W)
	const float TP_POS_P_Y = 5.0f;	// �O�l�̃J�����̎��_�ʒu(Y���W)
	const float TP_POS_P_Z = -10.0f;	// �O�l�̃J�����̎��_�ʒu(Z���W)
	const float TP_POS_R_X = 0.0f;		// �O�l�̃J�����̒����_�ʒu(X���W)
	const float TP_POS_R_Y = 2.0f;		// �O�l�̃J�����̒����_�ʒu(Y���W)
	const float TP_POS_R_Z = 0.0f;		// �O�l�̃J�����̒����_�ʒu(Z���W)


	CCamera g_camera;								// �J���� �C���X�^���X
}

//�}�E�X�̏�ԗ�
enum MOUSE_MODE
{
	NONE_DRAG,
	LEFT_DRAG,
	RIGHT_DRAG,
	MIDDLE_DRAG,

	MAX_MODE,
};


static MOUSE_MODE g_CameraMode = NONE_DRAG;		//�J�������[�h

CCamera* CCamera::m_pCamera = &g_camera;			// ���݂̃J����

/******************************************************************************
* 
* @brief      CCamera
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �R���X�g���N�^
* @attention  
******************************************************************************/
CCamera::CCamera()
{
	//�f�t�H���g�l�[��
	strcpy(m_cName, "Camera");
}

/******************************************************************************
* 
* @brief      Init
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ������
* @attention  
******************************************************************************/
void CCamera::Init()
{	
	m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);		// ���_
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);	// �����_
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);		// ����x�N�g��

	m_fAspectRatio = VIEW_ASPECT;			// �c����
	m_fFovY = VIEW_ANGLE;					// ����p(Degree)
	m_fNearZ = VIEW_NEAR_Z;					// �O���N���b�v����
	m_fFarZ = VIEW_FAR_Z;					// ����N���b�v����
	m_fMoveX = 0.0f;						//��������(X)
	m_fMoveY = 0.0f;						//��������(Y)
	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);	//�p�x

	//���_-�����_�̋���
	float fVecX, fVecZ;
	fVecX = m_vPos.x - m_vTarget.x;
	fVecZ = m_vPos.z - m_vTarget.z;
	m_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	//�J�����f�[�^���[�h
	LoadFile();

	m_bStart = false;

	//�X�J�C�{�b�N�X�ݒ�
	m_pSky = new CFBXFile;
	m_pSky->SetName("SkyDome");
	m_pSky->Load("data/Model/SkyDome/sky.fbx");
	m_pSky->SetPSType(UNLIT);

	CalcWorldMatrix();
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �I��
* @attention  
******************************************************************************/
void CCamera::Uninit()
{
	//�f�[�^�Z�[�u
	SaveFile();
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �X�V
* @attention  
******************************************************************************/
void CCamera::Update()
{
	//�Q�[���X�^�[�g���Ă��Ȃ��ꍇ
	if (!GLOBALDATA->GetStartFlag())
	{
		//�^�C�g���V�[���ł͂Ȃ��ꍇ
		if (SceneManager::Get()->GetID() != SCENE_TITLE)
		{
#pragma region �J��������
			static POINT mouseOld;			//�}�E�X���W
			static POINT mouseNew;			//�X�V��̃}�E�X���W

			//���_-�����_�̋���
			float fVecX, fVecY, fVecZ;
			fVecX = m_vPos.x - m_vTarget.x;
			fVecY = m_vPos.y - m_vTarget.y;
			fVecZ = m_vPos.z - m_vTarget.z;
			m_fLengthInterval = sqrtf(fVecX * fVecX + fVecY * fVecY + fVecZ * fVecZ);

#pragma region �}�E�X���͌n
			//�}�E�X�E�N���b�N------------------------------------------------------------------------
			if (GetAsyncKeyState(VK_RBUTTON) & 0x8000 && g_CameraMode != RIGHT_DRAG)
			{
				g_CameraMode = RIGHT_DRAG;//�}�E�X�E�h���b�O�t���O

				GetCursorPos(&mouseOld);//�}�E�X�̃X�N���[�����W�擾
			}
			else if (!GetAsyncKeyState(VK_RBUTTON) && g_CameraMode == RIGHT_DRAG)
			{
				g_CameraMode = NONE_DRAG;
			}
			//-----------------------------------------------------------------------------------------

			//�}�E�X���N���b�N------------------------------------------------------------------------
			if (GetAsyncKeyState(VK_MBUTTON) & 0x8000 && g_CameraMode != MIDDLE_DRAG)
			{
				g_CameraMode = MIDDLE_DRAG;//�}�E�X���h���b�O�t���O

				GetCursorPos(&mouseOld);//�}�E�X�̃X�N���[�����W�擾
			}
			else if (!GetAsyncKeyState(VK_MBUTTON) && g_CameraMode == MIDDLE_DRAG)
			{
				g_CameraMode = NONE_DRAG;
			}
			//-----------------------------------------------------------------------------------------

			//�}�E�X���N���b�N------------------------------------------------------------------------
			if (GetAsyncKeyState(VK_MENU) & 0x8000)
			{
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && g_CameraMode != LEFT_DRAG)
				{
					g_CameraMode = LEFT_DRAG;//�}�E�X���h���b�O�t���O

					GetCursorPos(&mouseOld);//�}�E�X�̃X�N���[�����W�擾
				}
				else if (!GetAsyncKeyState(VK_LBUTTON) && g_CameraMode == LEFT_DRAG)
				{
					g_CameraMode = NONE_DRAG;
				}
			}
			else if (!GetAsyncKeyState(VK_LBUTTON) && g_CameraMode == LEFT_DRAG)
			{
				g_CameraMode = NONE_DRAG;
			}
			//-----------------------------------------------------------------------------------------
#pragma endregion

			//�}�E�X���W�擾
			GetCursorPos(&mouseNew);

			//�}�E�X�̓�������
			float X = mouseNew.x - mouseOld.x;
			float Y = mouseNew.y - mouseOld.y;

#pragma region ���W�X�V
			switch (g_CameraMode)		//�J�������[�h���Ƃɏ���
			{
			case NONE_DRAG:
				break;
			case LEFT_DRAG:

				//���W�X�V---------------------------------------------
				XMFLOAT3 vZ(m_mtxView._13, m_mtxView._23, m_mtxView._33);

				m_vPos.x += Y * -vZ.x;
				m_vPos.y += Y * -vZ.y;
				m_vPos.z += Y * -vZ.z;
				//-----------------------------------------------------

				break;
			case RIGHT_DRAG:

				//�}�E�X�̈ړ���
				m_fMoveX += X * MOUSE_AXIS;
				m_fMoveY += Y * MOUSE_AXIS;

				//-----------------------------------------------------
				if (m_fMoveY > MOUSE_LIMIT)
					m_fMoveY = MOUSE_LIMIT;
				else if (m_fMoveY < -MOUSE_LIMIT)
					m_fMoveY = -MOUSE_LIMIT;
				//-----------------------------------------------------

				//���W�X�V---------------------------------------------
				m_vPos.x = m_vTarget.x + cosf(m_fMoveY) * cosf(m_fMoveX) * m_fLengthInterval;
				m_vPos.y = m_vTarget.y + sinf(m_fMoveY) * m_fLengthInterval;
				m_vPos.z = m_vTarget.z + cosf(m_fMoveY) * sinf(m_fMoveX) * m_fLengthInterval;
				//-----------------------------------------------------

			
				break;

			case MIDDLE_DRAG:

				//���W�X�V---------------------------------------------
				XMFLOAT3 vX(m_mtxView._11, m_mtxView._21, m_mtxView._31);
				XMFLOAT3 vY(m_mtxView._12, m_mtxView._22, m_mtxView._32);
				m_vTarget.x += X * -vX.x + Y * vY.x;
				m_vTarget.y += X * -vX.y + Y * vY.y;
				m_vTarget.z += X * -vX.z + Y * vY.z;
				m_vPos.x += X * -vX.x + Y * vY.x;
				m_vPos.y += X * -vX.y + Y * vY.y;
				m_vPos.z += X * -vX.z + Y * vY.z;
				//-----------------------------------------------------

				break;
			}
#pragma endregion

			//�}�E�X�X�V
			mouseOld = mouseNew;
#pragma endregion
		}
	}

	//�Q�[�����͂��܂����ꍇ
	if (GLOBALDATA->GetStartFlag() && m_pPlayer)
	{
		/*
		�J�������v���C���[�ɒǏ]����悤��
		*/
		XMMATRIX mW;
		XMFLOAT3 vNewUp, vNewTarget, vNewPos;
		mW = m_pPlayer->GetWorldMatrix();
		XMStoreFloat3(&vNewUp,
			XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), mW));
		XMStoreFloat3(&vNewTarget,
			XMVector3TransformCoord(
				XMVectorSet(TP_POS_R_X, TP_POS_R_Y, TP_POS_R_Z, 1.0f), mW));
		XMStoreFloat3(&vNewPos,
			XMVector3TransformCoord(
				XMVectorSet(TP_POS_P_X, TP_POS_P_Y, TP_POS_P_Z, 1.0f), mW));

		// ���_�����X�Ɉړ���ɋ߂Â���
		m_vUp.x = m_vUp.x * INERTIA_IN + vNewUp.x * INERTIA_OUT;
		m_vUp.y = m_vUp.y * INERTIA_IN + vNewUp.y * INERTIA_OUT;
		m_vUp.z = m_vUp.z * INERTIA_IN + vNewUp.z * INERTIA_OUT;
		// �����_�����X�Ɉړ���ɋ߂Â���
		m_vTarget.x = m_vTarget.x * INERTIA_IN + vNewTarget.x * INERTIA_OUT;
		m_vTarget.y = m_vTarget.y * INERTIA_IN + vNewTarget.y * INERTIA_OUT;
		m_vTarget.z = m_vTarget.z * INERTIA_IN + vNewTarget.z * INERTIA_OUT;
		// ���_�����X�Ɉړ���ɋ߂Â���
		m_vPos.x = m_vPos.x * INERTIA_IN + vNewPos.x * INERTIA_OUT;
		m_vPos.y = m_vPos.y * INERTIA_IN + vNewPos.y * INERTIA_OUT;
		m_vPos.z = m_vPos.z * INERTIA_IN + vNewPos.z * INERTIA_OUT;
	}

	//�Q�[�����n�܂����Ƃ��A�n�܂钼�O�̍��W���Z�[�u
	if (GLOBALDATA->GetStartFlag() && !m_bStart)
	{
		m_bStart = true;
		m_vOldPos = m_vPos;
		m_vOldTarget = m_vTarget;
	}

	//�Q�[�����I�������Ƃ��A�n�܂钼�O�̍��W�����[�h
	if (!GLOBALDATA->GetStartFlag() && m_bStart)
	{
		m_bStart = false;
		m_vPos = m_vOldPos;
		m_vTarget = m_vOldTarget;
	}

	// �}�g���b�N�X�X�V
	UpdateMatrix();
}

/******************************************************************************
* 
* @brief      Clear
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ��ʃN���A
* @attention  
******************************************************************************/
void CCamera::Clear()
{
	//�o�b�t�@�擾
	CBackBuffer *buffer = BACKBUFFER;

	//��ʂ��N���A
	float ClearColor[4] = { CLEAR_COLOR };
	ID3D11DeviceContext* pDC = BACKBUFFER->GetDeviceContext();
	pDC->ClearRenderTargetView(buffer->GetRenderTargetView(), ClearColor);
	pDC->ClearDepthStencilView(buffer->GetDepthStencilView() ,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

/******************************************************************************
* 
* @brief      SetSun
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ���C�g�̈ʒu�ɃJ������ݒ�
* @attention  �V���h�E�}�b�v�p
******************************************************************************/
void CCamera::SetSun()
{
	//���C�g�擾
	XMFLOAT3 lightDir = CLight::Get()->GetDir();
	XMFLOAT3 lightPos = CLight::Get()->GetPos();

	// ���z�̈ʒu�ɃJ����������Ƒz�肵�āA�e�N�X�`���ɏ�������
	// ���J�������߂����āA�e�N�X�`���ɏ������߂Ȃ��������o�Ă���ƁA�������\������Ȃ��̂ŁA
	// �@�J�����𗣂��Ĉʒu��ݒ肷��
	XMMATRIX sunView = XMMatrixLookAtLH(
		XMVectorSet(lightDir.x * lightPos.x, lightDir.y * -lightPos.y, lightDir.z * lightPos.z, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	XMMATRIX sunProj =XMMatrixOrthographicLH(
		1500.0f, 1500.0f * (9.0f / 16.0f), 0.2f, 3000.0f
	);

	// ���z�̌��͂قڂ܂��������ł���̂ŁA�p�x���A���������e�ł͐������Ȃ������ڂɂȂ�B
	DirectX::XMStoreFloat4x4(&m_mtxView,(sunView));
	DirectX::XMStoreFloat4x4(&m_mtxProj, (sunProj));
}

/******************************************************************************
* 
* @brief      Sky
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �X�J�C�{�b�N�X�`��
* @attention  
******************************************************************************/
void CCamera::Sky()
{
	//�o�b�t�@�擾
	CBackBuffer *buffer = BACKBUFFER;

	//�X�J�C�{�b�N�X������ꍇ
	if (m_pSky) {
		// Z�o�b�t�@����
		buffer->SetZBuffer(false);		
		CLight* pLight = CLight::Get();
		// ���C�e�B���O����
		pLight->SetDisable();	
		//�X�J�C�{�b�N�X�`��
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW, XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z));
		m_pSky->SetWorldMatrix(XMLoadFloat4x4(&mW));
		m_pSky->Draw();
		// ���C�e�B���O�L��
		pLight->SetEnable();
		// Z�o�b�t�@����
		buffer->SetZBuffer(true);		
	}
}

/******************************************************************************
* 
* @brief      UpdateMatrix
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �r���[/�v���W�F�N�V���� �}�g���b�N�X�X�V
* @attention  
******************************************************************************/
void CCamera::UpdateMatrix()
{
	//�}�g���b�N�X�X�V
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vPos), XMLoadFloat3(&m_vTarget), XMLoadFloat3(&m_vUp)));
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
}

/******************************************************************************
*
* @brief      SetWorldMatrix
* @param[in]  mtxWorld
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ���[���h�}�g���b�N�X�ݒ�
* @attention  
******************************************************************************/
void CCamera::SetWorldMatrix(XMFLOAT4X4& mtxWorld)
{
	//�ݒ�
	m_mtxWorld = mtxWorld;
	m_vPos = XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

/******************************************************************************
* 
* @brief      CalcWorldMatrix
* @return     XMFLOAT4X4 &
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ���_/�����_/����x�N�g�����烏�[���h �}�g���b�N�X�Z�o
* @attention  
******************************************************************************/
XMFLOAT4X4& CCamera::CalcWorldMatrix()
{
	//�x�N�g���v�Z
	XMVECTOR vecZ = XMVectorSet(m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z, 0.0f);
	XMFLOAT3 vZ;
	XMStoreFloat3(&vZ, XMVector3Normalize(vecZ));
	XMVECTOR vecY = XMLoadFloat3(&m_vUp);
	XMVECTOR vecX = XMVector3Normalize(XMVector3Cross(vecY, vecZ));
	XMFLOAT3 vX;
	XMStoreFloat3(&vX, vecX);
	vecY = XMVector3Normalize(XMVector3Cross(vecZ, vecX));
	XMFLOAT3 vY;
	XMStoreFloat3(&vY, vecY);

	//�ݒ�
	m_mtxWorld._11 = vX.x;
	m_mtxWorld._12 = vX.y;
	m_mtxWorld._13 = vX.z;
	m_mtxWorld._14 = 0.0f;
	m_mtxWorld._21 = vY.x;
	m_mtxWorld._22 = vY.y;
	m_mtxWorld._23 = vY.z;
	m_mtxWorld._24 = 0.0f;
	m_mtxWorld._31 = vZ.x;
	m_mtxWorld._32 = vZ.y;
	m_mtxWorld._33 = vZ.z;
	m_mtxWorld._34 = 0.0f;
	m_mtxWorld._41 = m_vPos.x;
	m_mtxWorld._42 = m_vPos.y;
	m_mtxWorld._43 = m_vPos.z;
	m_mtxWorld._44 = 1.0f;

	return m_mtxWorld;
}

/******************************************************************************
* 
* @brief      Set
* @param[in]  pCamera
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �J�����Z�b�g
* @attention  
******************************************************************************/
void CCamera::Set(CCamera* pCamera)
{
	//�J����������΃Z�b�g
	m_pCamera = (pCamera) ? pCamera : &g_camera;
	CCamera::Get()->UpdateMatrix();
}

/******************************************************************************
* 
* @brief      SaveFile
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �J�����Z�[�u
* @attention  
******************************************************************************/
void CCamera::SaveFile()
{
	FILE* fp;

	//data/save/m_cName.totsuka�̌`�ɂ���
	char path[256] = "data/save/";
	strcat(path, m_cName);
	strcat(path, ".totsuka");

	//�t�@�C���I�[�v��
	fopen_s(&fp, path, "wb");
	if (fp)		//�t�@�C�������������ꍇ
	{
		//�Z�[�u
		fwrite(this, sizeof(CCamera), 1, fp);
		fclose(fp);
	}
}

/******************************************************************************
* 
* @brief      LoadFile
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �J�������[�h
* @attention  
******************************************************************************/
void CCamera::LoadFile()
{
	//�Z�[�u�p�J����
	CCamera camera;

	//data/save/m_cName.totsuka�̌`�ɂ���
	char path[256] = "data/save/";
	strcat(path, m_cName);
	strcat(path, ".totsuka");

	FILE* fp;
	//�t�@�C���I�[�v��
	fopen_s(&fp, path, "rb");
	if (fp)			//�t�@�C�������������ꍇ
	{
		//���[�h
		fread(&camera, sizeof(CCamera), 1, fp);
		fclose(fp);
		*this = camera;
	}
}