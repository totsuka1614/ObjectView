/******************************************************************************
* 
* @file      light.cpp
* @brief     光源クラス
* @author    Totsuka Kensuke
* @date      2023/04/18
* @note      
* @attention 
* 
******************************************************************************/
#include "Light.h"

using namespace DirectX;

// グローバル変数
const XMFLOAT3 LIGHT0_POSITION = XMFLOAT3(500.0f, 500.0f, -500.0f);
const XMFLOAT3 LIGHT0_DIRECTION = XMFLOAT3(0.0f, -1.0f, 1.0f);
const XMFLOAT4 LIGHT0_DIFFUSE = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
const XMFLOAT4 LIGHT0_AMBIENT = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
const XMFLOAT4 LIGHT0_SPECULAR = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

CLight g_light;

// 静的メンバ変数
CLight* CLight::m_pLight = &g_light;

/******************************************************************************
* 
* @brief      CLight
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       コンストラクタ
* @attention  
* 
******************************************************************************/
CLight::CLight()
{
	//初期化
	Init();
}

/******************************************************************************
* 
* @brief      Init
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       初期化
* @attention  
* 
******************************************************************************/
void CLight::Init()
{
	//初期値設定
	XMFLOAT3 vDir = LIGHT0_DIRECTION;
	XMStoreFloat3(&m_direction, XMVector3Normalize(XMLoadFloat3(&vDir)));
	m_diffuse = LIGHT0_DIFFUSE;
	m_ambient = LIGHT0_AMBIENT;
	m_specular = LIGHT0_SPECULAR;
	m_bEnable = true;
	m_vPos = LIGHT0_POSITION;

}

/******************************************************************************
* 
* @brief      GetDir
* @return     XMFLOAT3 &
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       光源方向ゲッター
* @attention  
* 
******************************************************************************/
XMFLOAT3& CLight::GetDir()
{
	//光源が有効ならば
	if (m_bEnable) return m_direction;
	static XMFLOAT3 off(0.0f, 0.0f, 0.0f);
	return off;
}

/******************************************************************************
* 
* @brief      Get
* @return     CLight *
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       光源インスタンスゲッター
* @attention  
* 
******************************************************************************/
CLight* CLight::Get()
{
	return m_pLight;
}

/******************************************************************************
* 
* @brief      Set
* @param[in]  pLight
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       光源インスタンスセッター
* @attention  
* 
******************************************************************************/
void CLight::Set(CLight* pLight)
{
	m_pLight = (pLight) ? pLight : &g_light;
}
