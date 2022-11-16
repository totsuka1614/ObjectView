#include "ObjectBase.h"

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
	}

	*this = a;
}

void ObjectBase::LoadFile(ObjectBase save)
{
	*this = save;
}