//=============================================================================
//
// オブジェクトベース クラス定義 [ObjectBase.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#pragma once

#include "main.h"
#include <string.h>

class ObjectBase
{
public:
	ObjectBase() :m_vPos(0.0f, 0.0f, 0.0f), m_vScale(100.0f, 100.0f, 100.0f), m_vDegree(0.0f, 0.0f, 0.0f), m_bActive(false), m_bEnable(true), m_eType(BOX) 
	{
		//strcpy(m_cName, "Default");
	};
	virtual ~ObjectBase() {};

	XMFLOAT3& GetTransform(void) { return m_vPos; }
	XMFLOAT3& GetRotation(void) { return m_vDegree; }
	XMFLOAT3& GetScale(void) { return m_vScale; }
	ObjectType GetType(void) { return m_eType; }
	const char* GetName(void) { return m_cName; }
	bool& GetEnable(void) { return m_bEnable; }
	bool& GetActive(void) { return m_bActive; }

	void SetName(const char* name) { strcpy(m_cName, name); }
	void SaveFile();
	void LoadFile(ObjectBase save);
	void LoadFile();
	PSShaderType& GetPSType(void) { return m_PStype; }
protected:
	XMFLOAT3 m_vPos;
	XMFLOAT3 m_vDegree;
	XMFLOAT3 m_vScale;
	bool m_bActive;
	bool m_bEnable;
	char m_cName[256];
	ObjectType m_eType;

	PSShaderType m_PStype;
	VSShaderType m_VStype;
};
