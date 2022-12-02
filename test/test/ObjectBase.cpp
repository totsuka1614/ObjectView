#include "ObjectBase.h"
#include "ConstantBuffer.h"
#include "BackBuffer.h"
#include "CollisionList.h"


void ObjectBase::Init()
{
	ColList::Get()->SetObj(*this);
}

void ObjectBase::Update()
{
	BackBuffer* buffer = BackBuffer::GetBuffer();

	ConstantBuffer* add = new ConstantBuffer;
	add->Create(sizeof(XMFLOAT4));

	switch (m_PStype)
	{
	case DISSOLVE:
		buffer->SetTexture(buffer->GetTexture(DISSOLVE_MAP));
		add->Update(&m_fRate);
		add->SetPixelShader(1);
		break;
	case BUMPMAP:
		buffer->SetTexture(buffer->GetTexture(BUMP_MAP));
		break;
	case FOG:
		break;
	default:
		break;

	}

	delete add;
}

void ObjectBase::SaveFile()
{


	FILE* fp;

	char path[256] = "data/save/";
	strcat(path, m_cName);
	strcat(path, ".totsuka");

	fopen_s(&fp, path, "wb");
	if (fp)
	{
		fwrite(this, sizeof(ObjectBase), 1, fp);
		fclose(fp);
	}
}

void ObjectBase::LoadFile()
{
	ObjectBase a;

	char path[256] = "data/save/";
	strcat(path, m_cName);
	strcat(path, ".totsuka");

	FILE* fp;

	fopen_s(&fp, path, "rb");
	if (fp)
	{
		fread(&a, sizeof(ObjectBase), 1, fp);
		fclose(fp);
		*this = a;
	}

}

void ObjectBase::LoadFile(ObjectBase save)
{
	*this = save;
}

bool ObjectBase::CollisionToPoint(XMFLOAT2 pos)
{
	if (m_vPos.x - m_vScale.x / 2 < pos.x && m_vPos.x + m_vScale.x / 2 > pos.x)
	{
		if (m_vPos.y - m_vScale.y / 2 < pos.y && m_vPos.y + m_vScale.y / 2 > pos.y)
		{
			return true;
		}
	}

	return false;
}

bool ObjectBase::CollisionTo3D(ObjectBase* obj)
{
	if (obj == this) return false;
	if (!m_bEnable || !obj)
		return false;

	// ’†SÀ•W‚ð‹‚ß‚é
	XMFLOAT3 vA, vB;
	XMStoreFloat3(&vA,
		XMVector3TransformCoord(
			XMLoadFloat3(&m_vPos),
			m_mtxWorld));
	XMStoreFloat3(&vB,
		XMVector3TransformCoord(
			XMLoadFloat3(&obj->m_vPos),
			obj->m_mtxWorld));
	// Õ“Ë”»’è
	XMFLOAT3& vAr = m_vScale;
	XMFLOAT3& vBr = obj->GetScale();
	return vA.x - vAr.x <= vB.x + vBr.x &&
		vB.x - vBr.x <= vA.x + vAr.x &&
		vA.y - vAr.y <= vB.y + vBr.y &&
		vB.y - vBr.y <= vA.y + vAr.y &&
		vA.z - vAr.z <= vB.z + vBr.z &&
		vB.z - vBr.z <= vA.z + vAr.z;
}