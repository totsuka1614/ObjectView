//=============================================================================
//
// 境界ボックス クラス [Box.h]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#pragma once
#include "main.h"
#include "Mesh.h"

class Box : public CMesh
{
public:
	Box()
	{
	};
	~Box()
	{
	};

	void Init(XMFLOAT3 vSize);

	virtual void Update();
	virtual void Draw();

private:

};