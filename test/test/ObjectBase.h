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
#include "Texture.h"
#include "BackBuffer.h"

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
		m_bTexture = false;
		strcpy(m_cTexturePath, "data/Texture/.png");
		strcpy(m_cName, "Default");

	};
	virtual ~ObjectBase() {};

	virtual void Init();

	virtual void Update();

	XMFLOAT3& GetTransform(void) { return m_vPos; }
	XMFLOAT3& GetRotation(void) { return m_vDegree; }
	XMFLOAT3& GetScale(void) { return m_vScale; }
	XMFLOAT4& GetMaterial(void) { return m_Material.Diffuse; }
	ObjectType GetType(void) { return m_eType; }
	TAG& GetTag(void) { return m_eTag; }
	XMMATRIX& GetWorldMatrix(void) { return m_mtxWorld; }
	const char* GetName(void) { return m_cName; }
	bool& GetEnable(void) { return m_bEnable; }
	bool& GetActive(void) { return m_bActive; }
	bool& GetColFlag(void) { return m_bCol; }
	float& GetDissolveRate(void) { return m_fRate; }
	char* GetTexturePath(void) { return m_cTexturePath; }
	bool CollisionTo3D(ObjectBase*);

	void SetName(const char* name) { strcpy(m_cName, name); }
	void SaveFile();
	void LoadFile(ObjectBase save);
	void LoadFile();
	PSShaderType& GetPSType(void) { return m_PStype; }
	void SetPSType(PSShaderType type) { m_PStype = type; }

	char* GetFileName(void) { return m_cFileName; }
	void SetFileName(const char* a) { strcpy(m_cFileName, a); }
	void SetTextureFlag(bool a) { m_bTexture = a; }
	HRESULT SetTexture(void) { return CreateTextureFromFile(BACKBUFFER->GetDevice(), m_cTexturePath, &m_pTexrure); }
	void SetTexture(ID3D11ShaderResourceView* tex) { m_pTexrure = tex; }
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
	char m_cFileName[256];
	char m_cTexturePath[256];
	bool m_bTexture;
	ID3D11ShaderResourceView* m_pTexrure;
	TAG m_eTag;
};




#endif // !__OBJECTBASE_H__
