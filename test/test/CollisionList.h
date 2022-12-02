#ifndef __COLLISTION_H__
#define __COLLISTION_H__

#include "ObjectBase.h"
#include <vector>

class ColList
{
public:
	ColList() {};
	~ColList() {};

	bool CollisionAABB(ObjectBase*);
	bool CollisionOBB(ObjectBase*);

	static ColList* Get() { return m_pCol; }
	void SetObj(ObjectBase& a) { m_pObj.push_back(&a); }
private:
	static ColList* m_pCol;
	std::vector<ObjectBase*> m_pObj;

};

#endif