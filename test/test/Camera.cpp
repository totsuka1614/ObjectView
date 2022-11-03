//=============================================================================
//
// �J���� �N���X [Camera.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "Camera.h"
#include "BackBuffer.h"
#include "GUI.h"
#include <math.h>

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

	CCamera g_camera;								// �J���� �C���X�^���X
}

CCamera* CCamera::m_pCamera = &g_camera;			// ���݂̃J����

// �R���X�g���N�^
CCamera::CCamera()
{
	Init();
}

// ������
void CCamera::Init()
{

	m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);		// ���_
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);	// �����_
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);		// ����x�N�g��

	m_fAspectRatio = VIEW_ASPECT;			// �c����
	m_fFovY = VIEW_ANGLE;					// ����p(Degree)
	m_fNearZ = VIEW_NEAR_Z;					// �O���N���b�v����
	m_fFarZ = VIEW_FAR_Z;					// ����N���b�v����

	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float fVecX, fVecZ;
	fVecX = m_vPos.x - m_vTarget.x;
	fVecZ = m_vPos.z - m_vTarget.z;
	m_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	CalcWorldMatrix();
}

// �X�V
void CCamera::Update()
{
	static XMFLOAT3 range = XMFLOAT3(0.0f ,0.0f, 0.0f);

	static XMFLOAT3 vec;
	static POINT mouseOld;
	static POINT mouseNew;
	static bool bDrag = false;
	static float moveX = 0.0f;
	static float moveY = 0.0f;

	//�}�E�X�E�N���b�N
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000 && !bDrag)
	{
		range = XMFLOAT3(m_vPos.x - m_vTarget.x, m_vPos.y - m_vTarget.y, m_vPos.z - m_vTarget.z);
		vec = XMFLOAT3(sqrt(pow(range.x, 2) + pow(range.y, 2)), sqrt(pow(range.x, 2) + pow(range.z, 2)), sqrt(pow(range.y, 2) + pow(range.z, 2)));

		bDrag = true;//�}�E�X�E�h���b�O�t���O

		GetCursorPos(&mouseOld);//�}�E�X�̃X�N���[�����W�擾
	}
	else if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
	{
		bDrag = false;
	}
	if (bDrag)
	{
		GetCursorPos(&mouseNew);
		//range = XMFLOAT3(m_vPos.x - m_vTarget.x, m_vPos.y - m_vTarget.y, m_vPos.z - m_vTarget.z);

		moveX += (mouseNew.x - mouseOld.x) * 0.01f;
		moveY += (mouseNew.y - mouseOld.y) * 0.01f;

		if (moveY > 1.0f)
			moveY = 1.0f;
		else if (moveY <-1.0f)
			moveY = -1.0f;
		
		XMVECTOR a = XMLoadFloat3(&range);

		a = XMVector3Normalize(a);
		XMFLOAT3 b;
		XMStoreFloat3(&b, a);

		m_vPos.x = m_vTarget.x + cosf(moveX) * vec.y;
		m_vPos.y = m_vTarget.y + sinf(moveY) * vec.y/* (+ sinf(move)X) * vec.z*/;
		m_vPos.z = m_vTarget.z + sinf(moveX) * vec.y;
		mouseOld = mouseNew;
	}
	// �}�g���b�N�X�X�V
	UpdateMatrix();

	GUI::Get()->CameraCreate();
}

// ��ʃN���A
void CCamera::Clear()
{
	BackBuffer *buffer = BackBuffer::GetBuffer();

	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	ID3D11DeviceContext* pDC = BackBuffer::GetBuffer()->GetDeviceContext();
	pDC->ClearRenderTargetView(buffer->GetRenderTargetView(), ClearColor);
	pDC->ClearDepthStencilView(buffer->GetDepthStencilView() ,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

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
