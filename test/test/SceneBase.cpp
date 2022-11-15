#include "SceneBase.h"
#include "Utility.h"
#include "Box.h"

void SceneBase::DataSave(std::list<std::string> namelist)
{
	FILE* fp;

	char path[256] = "data/save/";
	strcat(path, "Scene");
	strcat(path, ".totsuka");

	fopen_s(&fp, path, "wb");
	if (fp)
	{
		for (auto list : namelist)
		{
			fwrite(list.data(), list.size(), 1, fp);
			fwrite("/", 1, 1, fp);

		}
		fclose(fp);
	}
}

void SceneBase::DataLoad(std::list<std::string>& namelist)
{

	char path[256] = "data/save/";
	strcat(path, "Scene");
	strcat(path, ".totsuka");

	FILE* fp;

	char* a = nullptr;

	fopen_s(&fp, path, "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		long Size = ftell(fp);
		a = new char[Size];
		fseek(fp, 0, SEEK_SET);
		fread(a, Size,1, fp);
		fclose(fp);
		// 「/」で分解　必要なし
		Split('/', a, namelist);

	}
	else
	{
		return;
	}

	for (auto name : namelist)
	{
		SAVE_DATA save;

		char path[256] = "data/save/";
		strcat(path, name.data());
		strcat(path, ".totsuka");

		fopen_s(&fp, path, "rb");
		if (fp)
		{
			fread(&save, sizeof(SAVE_DATA), 1, fp);

			fclose(fp);

			switch (save.eType)
			{
			case BOX:
				Create<Box>(name.data());
				GetComponent<Box>(name.data())->LoadFile(save);
				GetComponent<Box>(name.data())->SetName(name.data());
				GetComponent<Box>(name.data())->Init(XMFLOAT3(1.0f,1.0f,1.0f));
				break;
			case SPHERE:
				break;
			}
		}
	}
}