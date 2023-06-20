/******************************************************************************
* 
* @file      ObjectBase.h
* @brief     オブジェクトベースクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#ifndef __OBJECTBASE_H__
#define __OBJECTBASE_H__
//インクルード部
#include "main.h"
#include <string.h>
#include "Texture.h"
#include "BackBuffer.h"

//定数定義
#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,0.7f)
#define M_SPECULAR		XMFLOAT4(0.1f,0.1f,0.1f,1.0f)
#define M_AMBIENT		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

class CObjectBase
{
public:
	//コンストラクタ
	CObjectBase() :m_vPos(0.0f, 0.0f, 0.0f), m_vScale(10.0f, 10.0f, 10.0f), m_vDegree(0.0f, 0.0f, 0.0f), m_bActive(false), m_bEnable(true), m_eType(BOX), m_bCol(false)
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
		m_bNormalMap = false;
		m_pNormalMap = nullptr;
		strcpy(m_cTexturePath, "data/Texture/.png");
		strcpy(m_cNormalMapPath, "data/Texture/Normal/.png");
		strcpy(m_cName, "Default");

	};
	virtual ~CObjectBase() {};

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
	char* GetNormalMapPath(void) { return m_cNormalMapPath; }
	bool CollisionTo3D(CObjectBase*);

	void SetName(const char* name) { strcpy(m_cName, name); }
	void SaveFile();
	void LoadFile(CObjectBase save);
	void LoadFile();
	PSShaderType& GetPSType(void) { return m_PStype; }
	VSShaderType& GetVSType(void) { return m_VStype; }
	void SetPSType(PSShaderType type) { m_PStype = type; }
	void SetVSType(VSShaderType type) { m_VStype = type; }

	char* GetFileName(void) { return m_cFileName; }
	void SetFileName(const char* flag) { strcpy(m_cFileName, flag); }
	void SetTextureFlag(bool flag) { m_bTexture = flag; }
	void SetNormalMapFlag(bool flag) { m_bNormalMap = flag; }
	HRESULT SetTexture(void) { return CreateTextureFromFile(BACKBUFFER->GetDevice(), m_cTexturePath, &m_pTexrure); }
	HRESULT SetNormalMap(void) { return CreateTextureFromFile(BACKBUFFER->GetDevice(), m_cNormalMapPath, &m_pNormalMap); }
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
	//ディソルブ
	float m_fRate;

	char m_cFileName[256];
	char m_cTexturePath[256];
	char m_cNormalMapPath[256];
	bool m_bTexture;
	bool m_bNormalMap;
	ID3D11ShaderResourceView* m_pTexrure;
	ID3D11ShaderResourceView* m_pNormalMap;
	TAG m_eTag;
};




#endif // !__OBJECTBASE_H__
