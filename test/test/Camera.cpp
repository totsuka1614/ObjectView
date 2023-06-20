/******************************************************************************
* 
* @file      Camera.cpp
* @brief     カメラクラス
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
// 定数・マクロ定義
//*****************************************************************************
#define CLEAR_COLOR (0.117647f, 0.254902f, 0.352941f, 1.0f)			//画面クリアカラ―
#define MOUSE_LIMIT (1.5f)											//マウス制御点
#define MOUSE_AXIS (0.01f)											//マウス移動量係数
#define INERTIA_IN	(0.9f)											//慣性IN
#define INERTIA_OUT (1.0f - INERTIA_IN)								//慣性OUT

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

	const float TP_POS_P_X = 0.0f;		// 三人称カメラの視点位置(X座標)
	const float TP_POS_P_Y = 5.0f;	// 三人称カメラの視点位置(Y座標)
	const float TP_POS_P_Z = -10.0f;	// 三人称カメラの視点位置(Z座標)
	const float TP_POS_R_X = 0.0f;		// 三人称カメラの注視点位置(X座標)
	const float TP_POS_R_Y = 2.0f;		// 三人称カメラの注視点位置(Y座標)
	const float TP_POS_R_Z = 0.0f;		// 三人称カメラの注視点位置(Z座標)


	CCamera g_camera;								// カメラ インスタンス
}

//マウスの状態列挙
enum MOUSE_MODE
{
	NONE_DRAG,
	LEFT_DRAG,
	RIGHT_DRAG,
	MIDDLE_DRAG,

	MAX_MODE,
};


static MOUSE_MODE g_CameraMode = NONE_DRAG;		//カメラモード

CCamera* CCamera::m_pCamera = &g_camera;			// 現在のカメラ

/******************************************************************************
* 
* @brief      CCamera
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       コンストラクタ
* @attention  
******************************************************************************/
CCamera::CCamera()
{
	//デフォルトネーム
	strcpy(m_cName, "Camera");
}

/******************************************************************************
* 
* @brief      Init
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       初期化
* @attention  
******************************************************************************/
void CCamera::Init()
{	
	m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);		// 視点
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);	// 注視点
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);		// 上方ベクトル

	m_fAspectRatio = VIEW_ASPECT;			// 縦横比
	m_fFovY = VIEW_ANGLE;					// 視野角(Degree)
	m_fNearZ = VIEW_NEAR_Z;					// 前方クリップ距離
	m_fFarZ = VIEW_FAR_Z;					// 後方クリップ距離
	m_fMoveX = 0.0f;						//動いた量(X)
	m_fMoveY = 0.0f;						//動いた量(Y)
	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);	//角度

	//視点-注視点の距離
	float fVecX, fVecZ;
	fVecX = m_vPos.x - m_vTarget.x;
	fVecZ = m_vPos.z - m_vTarget.z;
	m_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	//カメラデータロード
	LoadFile();

	m_bStart = false;

	//スカイボックス設定
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
* @note       終了
* @attention  
******************************************************************************/
void CCamera::Uninit()
{
	//データセーブ
	SaveFile();
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       更新
* @attention  
******************************************************************************/
void CCamera::Update()
{
	//ゲームスタートしていない場合
	if (!GLOBALDATA->GetStartFlag())
	{
		//タイトルシーンではない場合
		if (SceneManager::Get()->GetID() != SCENE_TITLE)
		{
#pragma region カメラ操作
			static POINT mouseOld;			//マウス座標
			static POINT mouseNew;			//更新後のマウス座標

			//視点-注視点の距離
			float fVecX, fVecY, fVecZ;
			fVecX = m_vPos.x - m_vTarget.x;
			fVecY = m_vPos.y - m_vTarget.y;
			fVecZ = m_vPos.z - m_vTarget.z;
			m_fLengthInterval = sqrtf(fVecX * fVecX + fVecY * fVecY + fVecZ * fVecZ);

#pragma region マウス入力系
			//マウス右クリック------------------------------------------------------------------------
			if (GetAsyncKeyState(VK_RBUTTON) & 0x8000 && g_CameraMode != RIGHT_DRAG)
			{
				g_CameraMode = RIGHT_DRAG;//マウス右ドラッグフラグ

				GetCursorPos(&mouseOld);//マウスのスクリーン座標取得
			}
			else if (!GetAsyncKeyState(VK_RBUTTON) && g_CameraMode == RIGHT_DRAG)
			{
				g_CameraMode = NONE_DRAG;
			}
			//-----------------------------------------------------------------------------------------

			//マウス中クリック------------------------------------------------------------------------
			if (GetAsyncKeyState(VK_MBUTTON) & 0x8000 && g_CameraMode != MIDDLE_DRAG)
			{
				g_CameraMode = MIDDLE_DRAG;//マウス中ドラッグフラグ

				GetCursorPos(&mouseOld);//マウスのスクリーン座標取得
			}
			else if (!GetAsyncKeyState(VK_MBUTTON) && g_CameraMode == MIDDLE_DRAG)
			{
				g_CameraMode = NONE_DRAG;
			}
			//-----------------------------------------------------------------------------------------

			//マウス左クリック------------------------------------------------------------------------
			if (GetAsyncKeyState(VK_MENU) & 0x8000)
			{
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && g_CameraMode != LEFT_DRAG)
				{
					g_CameraMode = LEFT_DRAG;//マウス中ドラッグフラグ

					GetCursorPos(&mouseOld);//マウスのスクリーン座標取得
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

			//マウス座標取得
			GetCursorPos(&mouseNew);

			//マウスの動いた量
			float X = mouseNew.x - mouseOld.x;
			float Y = mouseNew.y - mouseOld.y;

#pragma region 座標更新
			switch (g_CameraMode)		//カメラモードごとに処理
			{
			case NONE_DRAG:
				break;
			case LEFT_DRAG:

				//座標更新---------------------------------------------
				XMFLOAT3 vZ(m_mtxView._13, m_mtxView._23, m_mtxView._33);

				m_vPos.x += Y * -vZ.x;
				m_vPos.y += Y * -vZ.y;
				m_vPos.z += Y * -vZ.z;
				//-----------------------------------------------------

				break;
			case RIGHT_DRAG:

				//マウスの移動量
				m_fMoveX += X * MOUSE_AXIS;
				m_fMoveY += Y * MOUSE_AXIS;

				//-----------------------------------------------------
				if (m_fMoveY > MOUSE_LIMIT)
					m_fMoveY = MOUSE_LIMIT;
				else if (m_fMoveY < -MOUSE_LIMIT)
					m_fMoveY = -MOUSE_LIMIT;
				//-----------------------------------------------------

				//座標更新---------------------------------------------
				m_vPos.x = m_vTarget.x + cosf(m_fMoveY) * cosf(m_fMoveX) * m_fLengthInterval;
				m_vPos.y = m_vTarget.y + sinf(m_fMoveY) * m_fLengthInterval;
				m_vPos.z = m_vTarget.z + cosf(m_fMoveY) * sinf(m_fMoveX) * m_fLengthInterval;
				//-----------------------------------------------------

			
				break;

			case MIDDLE_DRAG:

				//座標更新---------------------------------------------
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

			//マウス更新
			mouseOld = mouseNew;
#pragma endregion
		}
	}

	//ゲームがはじまった場合
	if (GLOBALDATA->GetStartFlag() && m_pPlayer)
	{
		/*
		カメラがプレイヤーに追従するように
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

		// 視点を徐々に移動先に近づける
		m_vUp.x = m_vUp.x * INERTIA_IN + vNewUp.x * INERTIA_OUT;
		m_vUp.y = m_vUp.y * INERTIA_IN + vNewUp.y * INERTIA_OUT;
		m_vUp.z = m_vUp.z * INERTIA_IN + vNewUp.z * INERTIA_OUT;
		// 注視点を徐々に移動先に近づける
		m_vTarget.x = m_vTarget.x * INERTIA_IN + vNewTarget.x * INERTIA_OUT;
		m_vTarget.y = m_vTarget.y * INERTIA_IN + vNewTarget.y * INERTIA_OUT;
		m_vTarget.z = m_vTarget.z * INERTIA_IN + vNewTarget.z * INERTIA_OUT;
		// 視点を徐々に移動先に近づける
		m_vPos.x = m_vPos.x * INERTIA_IN + vNewPos.x * INERTIA_OUT;
		m_vPos.y = m_vPos.y * INERTIA_IN + vNewPos.y * INERTIA_OUT;
		m_vPos.z = m_vPos.z * INERTIA_IN + vNewPos.z * INERTIA_OUT;
	}

	//ゲームが始まったとき、始まる直前の座標をセーブ
	if (GLOBALDATA->GetStartFlag() && !m_bStart)
	{
		m_bStart = true;
		m_vOldPos = m_vPos;
		m_vOldTarget = m_vTarget;
	}

	//ゲームが終了したとき、始まる直前の座標をロード
	if (!GLOBALDATA->GetStartFlag() && m_bStart)
	{
		m_bStart = false;
		m_vPos = m_vOldPos;
		m_vTarget = m_vOldTarget;
	}

	// マトリックス更新
	UpdateMatrix();
}

/******************************************************************************
* 
* @brief      Clear
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       画面クリア
* @attention  
******************************************************************************/
void CCamera::Clear()
{
	//バッファ取得
	CBackBuffer *buffer = BACKBUFFER;

	//画面をクリア
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
* @note       ライトの位置にカメラを設定
* @attention  シャドウマップ用
******************************************************************************/
void CCamera::SetSun()
{
	//ライト取得
	XMFLOAT3 lightDir = CLight::Get()->GetDir();
	XMFLOAT3 lightPos = CLight::Get()->GetPos();

	// 太陽の位置にカメラがあると想定して、テクスチャに書き込む
	// ※カメラが近すぎて、テクスチャに書き込めない部分が出てくると、正しく表示されないので、
	// 　カメラを離して位置を設定する
	XMMATRIX sunView = XMMatrixLookAtLH(
		XMVectorSet(lightDir.x * lightPos.x, lightDir.y * -lightPos.y, lightDir.z * lightPos.z, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	XMMATRIX sunProj =XMMatrixOrthographicLH(
		1500.0f, 1500.0f * (9.0f / 16.0f), 0.2f, 3000.0f
	);

	// 太陽の光はほぼまっすぐ飛んでくるので、角度が就く透視投影では正しくない見た目になる。
	DirectX::XMStoreFloat4x4(&m_mtxView,(sunView));
	DirectX::XMStoreFloat4x4(&m_mtxProj, (sunProj));
}

/******************************************************************************
* 
* @brief      Sky
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       スカイボックス描画
* @attention  
******************************************************************************/
void CCamera::Sky()
{
	//バッファ取得
	CBackBuffer *buffer = BACKBUFFER;

	//スカイボックスがある場合
	if (m_pSky) {
		// Zバッファ無効
		buffer->SetZBuffer(false);		
		CLight* pLight = CLight::Get();
		// ライティング無効
		pLight->SetDisable();	
		//スカイボックス描画
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW, XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z));
		m_pSky->SetWorldMatrix(XMLoadFloat4x4(&mW));
		m_pSky->Draw();
		// ライティング有効
		pLight->SetEnable();
		// Zバッファ無効
		buffer->SetZBuffer(true);		
	}
}

/******************************************************************************
* 
* @brief      UpdateMatrix
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ビュー/プロジェクション マトリックス更新
* @attention  
******************************************************************************/
void CCamera::UpdateMatrix()
{
	//マトリックス更新
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
* @note       ワールドマトリックス設定
* @attention  
******************************************************************************/
void CCamera::SetWorldMatrix(XMFLOAT4X4& mtxWorld)
{
	//設定
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
* @note       視点/注視点/上方ベクトルからワールド マトリックス算出
* @attention  
******************************************************************************/
XMFLOAT4X4& CCamera::CalcWorldMatrix()
{
	//ベクトル計算
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

	//設定
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
* @note       カメラセット
* @attention  
******************************************************************************/
void CCamera::Set(CCamera* pCamera)
{
	//カメラがあればセット
	m_pCamera = (pCamera) ? pCamera : &g_camera;
	CCamera::Get()->UpdateMatrix();
}

/******************************************************************************
* 
* @brief      SaveFile
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       カメラセーブ
* @attention  
******************************************************************************/
void CCamera::SaveFile()
{
	FILE* fp;

	//data/save/m_cName.totsukaの形にする
	char path[256] = "data/save/";
	strcat(path, m_cName);
	strcat(path, ".totsuka");

	//ファイルオープン
	fopen_s(&fp, path, "wb");
	if (fp)		//ファイルが見つかった場合
	{
		//セーブ
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
* @note       カメラロード
* @attention  
******************************************************************************/
void CCamera::LoadFile()
{
	//セーブ用カメラ
	CCamera camera;

	//data/save/m_cName.totsukaの形にする
	char path[256] = "data/save/";
	strcat(path, m_cName);
	strcat(path, ".totsuka");

	FILE* fp;
	//ファイルオープン
	fopen_s(&fp, path, "rb");
	if (fp)			//ファイルが見つかった場合
	{
		//ロード
		fread(&camera, sizeof(CCamera), 1, fp);
		fclose(fp);
		*this = camera;
	}
}