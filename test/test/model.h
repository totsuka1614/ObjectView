//=============================================================================
//
// モデル クラス定義 [Model.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef __MODEL_H__
#define __MODEL_H__

#include "main.h"
#include "vertex.h"
#include <string.h>
#include "FBX.h"

class Model : public FBXFile
{
public:
	Model(){}
	virtual ~Model(){}

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

protected:
	
private:
};

#endif // !__MODEL_H__
