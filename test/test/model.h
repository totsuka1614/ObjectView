//=============================================================================
//
// ���f�� �N���X��` [Model.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef __MODEL_H__
#define __MODEL_H__

#include "main.h"
#include "vertex.h"
#include <string.h>
#include "FBX.h"
#include "Box.h"

class Model : public FBXFile
{
public:
	Model(){}
	virtual ~Model();

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

protected:
	Box* m_Box; 
private:
};

#endif // !__MODEL_H__
