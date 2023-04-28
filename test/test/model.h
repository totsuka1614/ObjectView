/******************************************************************************
* 
* @file      model.h
* @brief     モデルクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#ifndef __MODEL_H__
#define __MODEL_H__
//インクルード部
#include "main.h"
#include "vertex.h"
#include <string.h>
#include "FBX.h"
#include "Box.h"

class CModel : public CFBXFile
{
public:
	CModel(){}
	virtual ~CModel();

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

protected:
	CBox* m_Box;		//当たり判定
private:
};

#endif // !__MODEL_H__
