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
#include "ObjectBase.h"

class FBXFile;
class Model : public ObjectBase
{
public:
	Model(){}
	virtual ~Model(){}

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	
	char* GetFileName(void) { return m_cFileName; }

protected:

	XMMATRIX m_mtxWorld;
	FBXFile* m_ModelData;
	
	char m_cFileName[256];
	
private:
};

#endif // !MODEL_H
