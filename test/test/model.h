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

class FBXFile;
class Model
{
public:
	Model() : m_vPos(0.0f,0.0f,0.0f),m_vDegree(0.0f,0.0f,0.0f),m_vScale(0.0f,0.0f,0.0f),m_ModelData(nullptr){}
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
private:
	XMFLOAT3 m_vPos;
	XMFLOAT3 m_vDegree;
	XMFLOAT3 m_vScale;
	XMMATRIX m_mtxWorld;
	FBXFile* m_ModelData;
	PSShaderType m_PStype;
	char m_cFileName[256];
};

#endif // !MODEL_H
