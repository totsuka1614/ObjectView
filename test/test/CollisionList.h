#ifndef __COLLISTION_H__
#define __COLLISTION_H__

#include "ObjectBase.h"
#include <vector>

#define COLLISION_AABB(object) ColList::Get()->CollisionAABB(object)
#define COLLISION_AABB_TAG(object,tag) ColList::Get()->CollisionAABB(object,tag)
#define COLLISION_OBB(object) ColList::Get()->CollisionOBB(object)

class ColList
{
public:
	ColList() {};
	~ColList() {};

	bool CollisionAABB(ObjectBase*);
	bool CollisionAABB(ObjectBase*,TAG);
	bool CollisionOBB(ObjectBase*);

	static ColList* Get() { return m_pCol; }
	void SetObj(ObjectBase& a) { m_pObj.push_back(&a); }
private:
	static ColList* m_pCol;
	std::vector<ObjectBase*> m_pObj;

};

#endif