//=============================================================================
//
// カメラ クラス [Camera.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "Camera.h"
#include "BackBuffer.h"
#include "GUI.h"
#include <math.h>

//*****************************************************************************
// グローバル変数
//*****************************************************************************
namespace {
	const float CAM_POS_P_X = 0.0f;					// カメラの視点初期位置(X座標)
	const float CAM_POS_P_Y = 250.0f;				// カメラの視点初期位置(Y座標)
	const float CAM_POS_P_Z = -560.0f;				// カメラの視点初期位置(Z座標)
	const float CAM_POS_R_X = 0.0f;					// カメラの注視点初期位置(X座標)
	const float CAM_POS_R_Y = 0.0f;					// カメラの注視点初期位置(Y座標)
	const float CAM_POS_R_Z = 0.0f;					// カメラの注視点初期位置(Z座標)
	const float VIEW_ANGLE = 45.0f;					// 視野角
	const float VIEW_ASPECT = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;	// アスペクト比
	const float VIEW_NEAR_Z = 1.0f;					// NearZ値
	const float VIEW_FAR_Z = 10000.0f;				// FarZ値

	CCamera g_camera;								// カメラ インスタンス
}

CCamera* CCamera::m_pCamera = &g_camera;			// 現在のカメラ

// コンストラクタ
CCamera::CCamera()
{
	Init();
}

// 初期化
void CCamera::Init()
{

	m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);		// 視点
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);	// 注視点
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);		// 上方ベクトル

	m_fAspectRatio = VIEW_ASPECT;			// 縦横比
	m_fFovY = VIEW_ANGLE;					// 視野角(Degree)
	m_fNearZ = VIEW_NEAR_Z;					// 前方クリップ距離
	m_fFarZ = VIEW_FAR_Z;					// 後方クリップ距離

	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float fVecX, fVecZ;
	fVecX = m_vPos.x - m_vTarget.x;
	fVecZ = m_vPos.z - m_vTarget.z;
	m_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	CalcWorldMatrix();
}

// 更新
void CCamera::Update()
{
	static XMFLOAT3 range = XMFLOAT3(0.0f ,0.0f, 0.0f);

	static XMFLOAT3 vec;
	static POINT mouseOld;
	static POINT mouseNew;
	static bool bDrag = false;
	static float moveX = 0.0f;
	static float moveY = 0.0f;

	//マウス右クリック
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000 && !bDrag)
	{
		range = XMFLOAT3(m_vPos.x - m_vTarget.x, m_vPos.y - m_vTarget.y, m_vPos.z - m_vTarget.z);
		vec = XMFLOAT3(sqrt(pow(range.x, 2) + pow(range.y, 2)), sqrt(pow(range.x, 2) + pow(range.z, 2)), sqrt(pow(range.y, 2) + pow(range.z, 2)));

		bDrag = true;//マウス右ドラッグフラグ

		GetCursorPos(&mouseOld);//マウスのスクリーン座標取得
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
	// マトリックス更新
	UpdateMatrix();

	GUI::Get()->CameraCreate();
}

// 画面クリア
void CCamera::Clear()
{
	BackBuffer *buffer = BackBuffer::GetBuffer();

	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	ID3D11DeviceContext* pDC = BackBuffer::GetBuffer()->GetDeviceContext();
	pDC->ClearRenderTargetView(buffer->GetRenderTargetView(), ClearColor);
	pDC->ClearDepthStencilView(buffer->GetDepthStencilView() ,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

// ビュー/プロジェクション マトリックス更新
void CCamera::UpdateMatrix()
{
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vPos), XMLoadFloat3(&m_vTarget), XMLoadFloat3(&m_vUp)));
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
}

// ワールド マトリックス設定
void CCamera::SetWorldMatrix(XMFLOAT4X4& mtxWorld)
{
	m_mtxWorld = mtxWorld;
	m_vPos = XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

// 視点/注視点/上方ベクトルからワールド マトリックス算出
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

// カメラ インスタンス設定
void CCamera::Set(CCamera* pCamera)
{
	m_pCamera = (pCamera) ? pCamera : &g_camera;
}
