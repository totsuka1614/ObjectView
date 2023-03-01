//=============================================================================
//
// 光源クラス [Light.h]
// Author : KENSUKE TOTSUKA
//
//=============================================================================
#pragma once
#include "main.h"
#include "billboard.h"

class CLight
{
private:
	XMFLOAT4 m_diffuse;
	XMFLOAT4 m_ambient;
	XMFLOAT4 m_specular;
	XMFLOAT3 m_direction;
	bool m_bEnable;			// ライティング有効/無効
	XMFLOAT3 m_vPos;
	CBillboard* m_pObj;
	static CLight* m_pLight;

public:
	CLight();

	void Init();
	XMFLOAT4& GetDiffuse() { return m_diffuse; }
	XMFLOAT4& GetAmbient() { return m_ambient; }
	XMFLOAT4& GetSpecular() { return m_specular; }
	XMFLOAT3& GetDir();
	XMFLOAT3& GetPos() { return m_vPos; }
	void SetEnable(bool bEnable = true) { m_bEnable = bEnable; }
	void SetDisable(bool bDisable = true) { m_bEnable = !bDisable; }

	CBillboard* GetObj() { return m_pObj; }

	static CLight* Get();
	static void Set(CLight* pLight = nullptr);
};
