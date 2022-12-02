//=============================================================================
//
// オブジェクトベース クラス定義 [ObjectBase.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef __OBJECTBASE_H__
#define __OBJECTBASE_H__


#include "main.h"
#include <string.h>

//定数定義
#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,0.7f)
#define M_SPECULAR		XMFLOAT4(0.1f,0.1f,0.1f,1.0f)
#define M_AMBIENT		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

class ObjectBase
{
public:
	ObjectBase() :m_vPos(0.0f, 0.0f, 0.0f), m_vScale(100.0f, 100.0f, 100.0f), m_vDegree(0.0f, 0.0f, 0.0f), m_bActive(false), m_bEnable(true), m_eType(BOX), m_bCol(false)
	{
		m_VStype = VERTEX;
		m_PStype = LAMBERT;
		// マテリアルの初期設定
		m_Material.Diffuse = M_DIFFUSE;
		m_Material.Ambient = M_AMBIENT;
		m_Material.Specular = M_SPECULAR;
		m_Material.Power = 0.0f;
		m_Material.Emissive = M_EMISSIVE;
		m_fRate = 0.0f;
		//strcpy(m_cName, "Default");

	};
	virtual ~ObjectBase() {};

	virtual void Init();

	virtual void Update();

	XMFLOAT3& GetTransform(void) { return m_vPos; }
	XMFLOAT3& GetRotation(void) { return m_vDegree; }
	XMFLOAT3& GetScale(void) { return m_vScale; }
	XMFLOAT4& GetMaterial(void) { return m_Material.Diffuse; }
	ObjectType GetType(void) { return m_eType; }
	XMMATRIX& GetWorldMatrix(void) { return m_mtxWorld; }
	const char* GetName(void) { return m_cName; }
	bool& GetEnable(void) { return m_bEnable; }
	bool& GetActive(void) { return m_bActive; }
	bool& GetColFlag(void) { return m_bCol; }
	float& GetDissolveRate(void) { return m_fRate; } 
	bool CollisionToPoint(XMFLOAT2 pos);
	bool CollisionTo2D(XMFLOAT2 pos,XMFLOAT2 size);
	bool CollisionTo3D(ObjectBase*);

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
	MATERIAL m_Material;
	XMMATRIX m_mtxWorld;
	PSShaderType m_PStype;
	VSShaderType m_VStype;

	bool m_bCol;
	//ディソルブレ―と
	float m_fRate;
};




#endif // !__OBJECTBASE_H__
