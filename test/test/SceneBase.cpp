/******************************************************************************
* 
* @file      SceneBase.cpp
* @brief     シーンベース
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      シーンのロード、セーブの実装に成功
* @attention 
* 
******************************************************************************/

#include "SceneBase.h"
#include "Utility.h"
#include "Box.h"
#include "model.h"

/******************************************************************************
* 
* @brief      DataSave
* @param[in]  namelist
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       データをセーブする
* @attention  
******************************************************************************/
void SceneBase::DataSave(std::list<std::string> namelist)
{
	/*
	data/save/シーン名.totsukaのファイルを検索し、セーブする
	その中身は オブジェクト名/オブジェクト名/オブジェクト名......と記述する
	*/
	FILE* fp;

	char path[256] = "data/save/";
	strcat(path, "Scene");
	strcat(path, ".totsuka");  //  data/save/シーン名.totsuka

	//ファイルオープン
	fopen_s(&fp, path, "wb");
	
	if (fp)	//ファイルが見つかった場合
	{
		for (auto list : namelist) //オブジェクトの数分繰り返す
		{
			fwrite(list.data(), list.size(), 1, fp);	//書き込み
			fwrite("/", 1, 1, fp);						//区切り
		}
		//ファイルクローズ
		fclose(fp);
	}
}

/******************************************************************************
* 
* @brief      DataLoad
* @param[in]  namelist
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       データをロードする
* @attention  
******************************************************************************/
void SceneBase::DataLoad(std::list<std::string>& namelist)
{
	/*
	data/save/シーン名.totsukaのファイルを検索し、ロードする
	オブジェクト名/オブジェクト名/オブジェクト名......となっているので、分解してnamelistに入れる
	その後、data/save/namelist.totsuka のデータを参照し、全てのオブジェクトをロードする
	*/

	char path[256] = "data/save/";
	strcat(path, "Scene");
	strcat(path, ".totsuka"); //  data/save/シーン名.totsuka

	FILE* fp;

	char* list = nullptr;

	//ファイルオープン
	fopen_s(&fp, path, "rb");
	
	if (fp) //ファイルが見つかった場合
	{
		//データサイズ取得
		fseek(fp, 0, SEEK_END); //末尾に移動
		long Size = ftell(fp);  //サイズ取得
		list = new char[Size];  //サイズ分のメモリを確保
		fseek(fp, 0, SEEK_SET); //その後先頭へ移動

		//読み込み
		fread(list, Size, 1, fp);
		
		//ファイルクローズ
		fclose(fp);

		// 「/」で分解
		Split('/', list, namelist);

	}
	else //ファイルが見つからなかった場合
		return;		//終了

	for (auto name : namelist)  //オブジェクトの数分繰り返す
	{
		//オブジェクトベースにロード
		ObjectBase save;

		char path[256] = "data/save/";
		strcat(path, name.data());
		strcat(path, ".totsuka");		//data/save/namelist.totsuka

		//ファイルオープン
		fopen_s(&fp, path, "rb");
		
		if (fp) //ファイルが見つかった場合
		{
			//オブジェクトをロード
			fread(&save, sizeof(ObjectBase), 1, fp);

			//ファイルクローズ
			fclose(fp);

			switch (save.GetType())	//オブジェクトタイプ別に生成
			{
			case BOX:	//ボックス
				Create<Box>(name.data());	//生成
				GetComponent<Box>(name.data())->LoadFile(save);		//読み込んだデータをセット
				GetComponent<Box>(name.data())->Init(XMFLOAT3(1.0f,1.0f,1.0f));	//初期化(size 1.0f)
				break;
			case SPHERE: //スフィア(未実装)
				break;
			case FBX:  //FBXモデル
				Create<Model>(name.data());	//生成
				GetComponent<Model>(name.data())->LoadFile(save);	//読み込んだデータをセット
				GetComponent<Model>(name.data())->Init();			//初期化
				break;
			}
		}
	}
}