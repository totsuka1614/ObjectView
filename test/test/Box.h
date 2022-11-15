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
		m_vTarget = new TARGET_TRANSFORM;
		m_vTarget->pos = new XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vTarget->scale = new XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vTarget->deglee = new XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vMove.pos = new XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vMove.scale = new XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vMove.deglee = new XMFLOAT3(0.0f, 0.0f, 0.0f);
		SetName("BoxCollider");
	};
	~Box()
	{
	};

	void Init(XMFLOAT3 vSize);

	virtual void Update();
	virtual void Draw();
	virtual void ColliderDraw();

	//XMFLOAT3& GetMove() { return m_vMove; }
	void SetTarget(TARGET_TRANSFORM& pos) 
	{ 
		SAVE_TRANSFORM save;

		char path[256] = "data/save/";
		strcat(path, GetName());
		strcat(path, ".totsuka");

		FILE* fp;

		fopen_s(&fp, path, "rb");
		if (fp)
		{
			fread(&save, sizeof(SAVE_TRANSFORM), 1, fp);
			fclose(fp);

			m_vPos = save.pos;
			m_vScale = save.scale;
			m_vDegree = save.deglee;
		}
		m_vTarget = &pos;
	}

private:
	TARGET_TRANSFORM* m_vTarget;
	TARGET_TRANSFORM m_vMove;
};