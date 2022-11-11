//=============================================================================
//
// モデル クラス定義 [Model.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef MODEL_H
#define MODEL_H

#include "main.h"
#include "vertex.h"
#include "FBX.h"
#include "Box.h"
#include <string.h>

class FBXFile;
class Model
{
public:
	Model() : m_vPos(0.0f,0.0f,0.0f),m_vDegree(0.0f,0.0f,0.0f),m_vScale(0.0f,0.0f,0.0f),m_ModelData(nullptr)
	{
		strcpy(m_cName, "Player");
	}
	virtual ~Model(){}

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	XMFLOAT3& GetTransform(void) { return m_vPos; }
	XMFLOAT3& GetRotation(void) { return m_vDegree; }
	XMFLOAT3& GetScale(void) { return m_vScale; }
	PSShaderType& GetPSType(void) { return m_PStype; }
	char* GetFileName(void) { return m_cFileName; }
	char* GetName(void) { return m_cName; }
	bool& GetActive(void) { return bActive; }
private:

	void LoadFile();

	XMFLOAT3 m_vPos;
	XMFLOAT3 m_vDegree;
	XMFLOAT3 m_vScale;
	XMMATRIX m_mtxWorld;
	FBXFile* m_ModelData;
	Box* m_Box;
	PSShaderType m_PStype;
	VSShaderType m_VStype;
	char m_cFileName[256];
	char m_cName[256];
	bool bActive;
};

#endif // !MODEL_H
