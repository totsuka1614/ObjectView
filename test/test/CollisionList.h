/******************************************************************************
* 
* @file      CollisionList.h
* @brief     コリジョンリストクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#ifndef __COLLISTION_H__
#define __COLLISTION_H__
//インクルード部
#include "ObjectBase.h"
#include <vector>
//マクロ
#define COLLISION_AABB(object) CColList::Get()->CollisionAABB(object)
#define COLLISION_AABB_TAG(object,tag) CColList::Get()->CollisionAABB(object,tag)
#define COLLISION_OBB(object) CColList::Get()->CollisionOBB(object)

class CColList
{
public:
	CColList() {};
	~CColList() {};

	bool CollisionAABB(CObjectBase*);
	bool CollisionAABB(CObjectBase*,TAG);
	bool CollisionOBB(CObjectBase*);

	static CColList* Get() { return m_pCol; }
	void SetObj(CObjectBase& a) { m_pObj.push_back(&a); }
private:
	static CColList* m_pCol;
	std::vector<CObjectBase*> m_pObj;

};

#endif