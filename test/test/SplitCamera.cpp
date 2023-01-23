#include "SplitCamera.h"

//スプリットカメラ-------------------------------------------------------------
const float SIDE_POS_P_X = 750.0f;					// カメラの視点初期位置(X座標)
const float SIDE_POS_P_Y = 50.0f;					// カメラの視点初期位置(Y座標)
const float SIDE_POS_P_Z = 0.0f;					// カメラの視点初期位置(Z座標)
const float SIDE_POS_R_X = 0.0f;					// カメラの注視点初期位置(X座標)
const float SIDE_POS_R_Y = 50.0f;					// カメラの注視点初期位置(Y座標)
const float SIDE_POS_R_Z = 0.0f;					// カメラの注視点初期位置(Z座標)

const float FRONT_POS_P_X = 0.0f;					// カメラの視点初期位置(X座標)
const float FRONT_POS_P_Y = 50.0f;				// カメラの視点初期位置(Y座標)
const float FRONT_POS_P_Z = -750.0f;				// カメラの視点初期位置(Z座標)
const float FRONT_POS_R_X = 0.0f;					// カメラの注視点初期位置(X座標)
const float FRONT_POS_R_Y = 50.0f;					// カメラの注視点初期位置(Y座標)
const float FRONT_POS_R_Z = 0.0f;					// カメラの注視点初期位置(Z座標)

const float UP_POS_P_X = 0.1f;					// カメラの視点初期位置(X座標)
const float UP_POS_P_Y = 1000.0f;				// カメラの視点初期位置(Y座標)
const float UP_POS_P_Z = 0.0f;				// カメラの視点初期位置(Z座標)
const float UP_POS_R_X = 0.0f;					// カメラの注視点初期位置(X座標)
const float UP_POS_R_Y = 0.0f;					// カメラの注視点初期位置(Y座標)
const float UP_POS_R_Z = 0.0f;					// カメラの注視点初期位置(Z座標)
//-------------------------------------------------------------------------------------

void CameraSplit::Init()
{
	CCamera::Init();
	m_eMode = NOW_VIEW;
}

void CameraSplit::SetMode(SplitMode mode)
{
	m_eMode = mode;

	switch (m_eMode)
	{
	case NOW_VIEW:
		Set(CCamera::Get());
		break;
	case SIDE_VIEW:
		m_vPos.x	= SIDE_POS_P_X;
		m_vPos.y	= SIDE_POS_P_Y;
		m_vPos.z	= SIDE_POS_P_Z;
		m_vTarget.x	= SIDE_POS_R_X;
		m_vTarget.y	= SIDE_POS_R_Y;
		m_vTarget.z	= SIDE_POS_R_Z;
		break;
	case FRONT_VIEW:
		m_vPos.x = FRONT_POS_P_X;
		m_vPos.y = FRONT_POS_P_Y;
		m_vPos.z = FRONT_POS_P_Z;
		m_vTarget.x = FRONT_POS_R_X;
		m_vTarget.y = FRONT_POS_R_Y;
		m_vTarget.z = FRONT_POS_R_Z;
		break;
	case UP_VIEW:
		m_vPos.x = UP_POS_P_X;
		m_vPos.y = UP_POS_P_Y;
		m_vPos.z = UP_POS_P_Z;
		m_vTarget.x = UP_POS_R_X;
		m_vTarget.y = UP_POS_R_Y;
		m_vTarget.z = UP_POS_R_Z;
		break;
	default:
		break;
	}

	// マトリックス更新
	UpdateMatrix();
}