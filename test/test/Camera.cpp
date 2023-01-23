//=============================================================================
//
// �J���� �N���X [Camera.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
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

enum MOUSE_MODE
{
	NONE_DRAG,
	LEFT_DRAG,
	RIGHT_DRAG,
	MIDDLE_DRAG,

	MAX_MODE,
};

static MOUSE_MODE g_CameraMode = NONE_DRAG;

CCamera* CCamera::m_pCamera = &g_camera;			// ���݂̃J����

// �R���X�g���N�^
CCamera::CCamera()
{
	strcpy(m_cName, "Camera");
}

// ������6
void CCamera::Init()
{	
	m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);		// ���_
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);	// �����_
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);		// ����x�N�g��

	m_fAspectRatio = VIEW_ASPECT;			// �c����
	m_fFovY = VIEW_ANGLE;					// ����p(Degree)
	m_fNearZ = VIEW_NEAR_Z;					// �O���N���b�v����
	m_fFarZ = VIEW_FAR_Z;					// ����N���b�v����
	m_fMoveX = 0.0f;
	m_fMoveY = 0.0f;
	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float fVecX, fVecZ;
	fVecX = m_vPos.x - m_vTarget.x;
	fVecZ = m_vPos.z - m_vTarget.z;
	m_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	LoadFile();

	m_bStart = false;

	m_pSky = new FBXFile;
	m_pSky->SetName("SkyDome");
	m_pSky->Load("data/Model/SkyDome/sky.fbx");
	m_pSky->SetPSType(UNLIT);

	CalcWorldMatrix();

}

void CCamera::Init(XMFLOAT3 pos,XMFLOAT3 target)
{

	m_vPos = pos;		// ���_
	m_vTarget = target;	// �����_
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);		// ����x�N�g��

	m_fAspectRatio = VIEW_ASPECT;			// �c����
	m_fFovY = VIEW_ANGLE;					// ����p(Degree)
	m_fNearZ = VIEW_NEAR_Z;					// �O���N���b�v����
	m_fFarZ = VIEW_FAR_Z;					// ����N���b�v����
	m_fMoveX = 0.0f;
	m_fMoveY = 0.0f;
	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float fVecX, fVecZ;
	fVecX = m_vPos.x - m_vTarget.x;
	fVecZ = m_vPos.z - m_vTarget.z;
	m_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	CalcWorldMatrix();

	UpdateMatrix();
}

void CCamera::Uninit()
{
	SaveFile();
}

// �X�V
void CCamera::Update()
{
	if (!GLOBALDATA->GetStartFlag())
	{
		if (SceneManager::Get()->GetID() != SCENE_TITLE)
		{
#pragma region �J��������
			static XMFLOAT3 range = XMFLOAT3(0.0f, 0.0f, 0.0f);

			static XMFLOAT3 vec;
			static POINT mouseOld;
			static POINT mouseNew;

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

			GetCursorPos(&mouseNew);

			float X = mouseNew.x - mouseOld.x;
			float Y = mouseNew.y - mouseOld.y;

#pragma region ���W�X�V
			switch (g_CameraMode)
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
				m_fMoveX += X * 0.01f;
				m_fMoveY += Y * 0.01f;

				//-----------------------------------------------------
				if (m_fMoveY > 1.5f)
					m_fMoveY = 1.5f;
				else if (m_fMoveY < -1.5f)
					m_fMoveY = -1.5f;
				//-----------------------------------------------------

				//���W�X�V---------------------------------------------
				m_vPos.x = m_vTarget.x + cosf(m_fMoveY) * cosf(m_fMoveX) * m_fLengthInterval;
				m_vPos.y = m_vTarget.y + sinf(m_fMoveY) * m_fLengthInterval;
				m_vPos.z = m_vTarget.z + cosf(m_fMoveY) * sinf(m_fMoveX) * m_fLengthInterval;
				//-----------------------------------------------------

			/*	if (GetAsyncKeyState('W') & 0x8000)
				{
					XMFLOAT3 vZ(m_mtxView._13, m_mtxView._23, m_mtxView._33);

					m_vTarget.x += vZ.x * 2.0f;
					m_vTarget.y += vZ.y * 2.0f;
					m_vTarget.z += vZ.z * 2.0f;
					m_vPos.x += vZ.x * 2.0f;
					m_vPos.y += vZ.y * 2.0f;
					m_vPos.z += vZ.z * 2.0f;
				}
				if (GetAsyncKeyState('D') & 0x8000)
				{
					XMFLOAT3 vX(m_mtxView._11, m_mtxView._21, m_mtxView._31);

					m_vTarget.x += vX.x * 2.0f;
					m_vTarget.y += vX.y * 2.0f;
					m_vTarget.z += vX.z * 2.0f;
					m_vPos.x += vX.x * 2.0f;
					m_vPos.y += vX.y * 2.0f;
					m_vPos.z += vX.z * 2.0f;
				}
				if (GetAsyncKeyState('S') & 0x8000)
				{
					XMFLOAT3 vZ(m_mtxView._13, m_mtxView._23, m_mtxView._33);

					m_vTarget.x -= vZ.x * 2.0f;
					m_vTarget.y -= vZ.y * 2.0f;
					m_vTarget.z -= vZ.z * 2.0f;
					m_vPos.x -= vZ.x * 2.0f;
					m_vPos.y -= vZ.y * 2.0f;
					m_vPos.z -= vZ.z * 2.0f;
				}
				if (GetAsyncKeyState('A') & 0x8000)
				{
					XMFLOAT3 vX(m_mtxView._11, m_mtxView._21, m_mtxView._31);

					m_vTarget.x -= vX.x * 2.0f;
					m_vTarget.y -= vX.y * 2.0f;
					m_vTarget.z -= vX.z * 2.0f;
					m_vPos.x -= vX.x * 2.0f;
					m_vPos.y -= vX.y * 2.0f;
					m_vPos.z -= vX.z * 2.0f;
				}*/
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

	if (GLOBALDATA->GetStartFlag() && m_pPlayer)
	{
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
		m_vUp.x = m_vUp.x * 0.9f + vNewUp.x * 0.1f;
		m_vUp.y = m_vUp.y * 0.9f + vNewUp.y * 0.1f;
		m_vUp.z = m_vUp.z * 0.9f + vNewUp.z * 0.1f;
		m_vTarget.x = m_vTarget.x * 0.9f + vNewTarget.x * 0.1f;
		m_vTarget.y = m_vTarget.y * 0.9f + vNewTarget.y * 0.1f;
		m_vTarget.z = m_vTarget.z * 0.9f + vNewTarget.z * 0.1f;
		m_vPos.x = m_vPos.x * 0.9f + vNewPos.x * 0.1f;
		m_vPos.y = m_vPos.y * 0.9f + vNewPos.y * 0.1f;
		m_vPos.z = m_vPos.z * 0.9f + vNewPos.z * 0.1f;
	}

	if (GLOBALDATA->GetStartFlag() && !m_bStart)
	{
		m_bStart = true;
		m_vOldPos = m_vPos;
		m_vOldTarget = m_vTarget;
	}

	if (!GLOBALDATA->GetStartFlag() && m_bStart)
	{
		m_bStart = false;
		m_vPos = m_vOldPos;
		m_vTarget = m_vOldTarget;
	}

	// �}�g���b�N�X�X�V
	UpdateMatrix();
}

// ��ʃN���A
void CCamera::Clear()
{
	BackBuffer *buffer = BACKBUFFER;

	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	ID3D11DeviceContext* pDC = BACKBUFFER->GetDeviceContext();
	pDC->ClearRenderTargetView(buffer->GetRenderTargetView(), ClearColor);
	pDC->ClearDepthStencilView(buffer->GetDepthStencilView() ,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (m_pSky) {
		buffer->SetZBuffer(false);		// Z�o�b�t�@����
		CLight* pLight = CLight::Get();
		pLight->SetDisable();	// ���C�e�B���O����
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW, XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z));
		m_pSky->SetWorldMatrix(XMLoadFloat4x4(&mW));
		m_pSky->Draw();
		pLight->SetEnable();	// ���C�e�B���O�L��
		buffer->SetZBuffer(true);		// Z�o�b�t�@����
	}

}

void CCamera::Sky()
{
	BackBuffer *buffer = BACKBUFFER;

	if (m_pSky) {
		buffer->SetZBuffer(false);		// Z�o�b�t�@����
		CLight* pLight = CLight::Get();
		pLight->SetDisable();	// ���C�e�B���O����
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW, XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z));
		m_pSky->SetWorldMatrix(XMLoadFloat4x4(&mW));
		m_pSky->Draw();
		pLight->SetEnable();	// ���C�e�B���O�L��
		buffer->SetZBuffer(true);		// Z�o�b�t�@����
	}
}

// �r���[/�v���W�F�N�V���� �}�g���b�N�X�X�V
void CCamera::UpdateMatrix()
{
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vPos), XMLoadFloat3(&m_vTarget), XMLoadFloat3(&m_vUp)));
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
}

// ���[���h �}�g���b�N�X�ݒ�
void CCamera::SetWorldMatrix(XMFLOAT4X4& mtxWorld)
{
	m_mtxWorld = mtxWorld;
	m_vPos = XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

// ���_/�����_/����x�N�g�����烏�[���h �}�g���b�N�X�Z�o
XMFLOAT4X4& CCamera::CalcWorldMatrix()
{
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

// �J���� �C���X�^���X�ݒ�
void CCamera::Set(CCamera* pCamera)
{
	m_pCamera = (pCamera) ? pCamera : &g_camera;
}


void CCamera::SaveFile()
{
	FILE* fp;

	char path[256] = "data/save/";
	strcat(path, m_cName);
	strcat(path, ".totsuka");

	fopen_s(&fp, path, "wb");
	if (fp)
	{
		fwrite(this, sizeof(CCamera), 1, fp);
		fclose(fp);
	}
}

void CCamera::LoadFile()
{
	CCamera a;

	char path[256] = "data/save/";
	strcat(path, m_cName);
	strcat(path, ".totsuka");

	FILE* fp;

	fopen_s(&fp, path, "rb");
	if (fp)
	{
		fread(&a, sizeof(CCamera), 1, fp);
		fclose(fp);
		*this = a;
	}
}