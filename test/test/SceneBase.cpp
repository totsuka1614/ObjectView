/******************************************************************************
* 
* @file      SceneBase.cpp
* @brief     �V�[���x�[�X
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      �V�[���̃��[�h�A�Z�[�u�̎����ɐ���
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
* @note       �f�[�^���Z�[�u����
* @attention  
******************************************************************************/
void SceneBase::DataSave(std::list<std::string> namelist)
{
	/*
	data/save/�V�[����.totsuka�̃t�@�C�����������A�Z�[�u����
	���̒��g�� �I�u�W�F�N�g��/�I�u�W�F�N�g��/�I�u�W�F�N�g��......�ƋL�q����
	*/
	FILE* fp;

	char path[256] = "data/save/";
	strcat(path, "Scene");
	strcat(path, ".totsuka");  //  data/save/�V�[����.totsuka

	//�t�@�C���I�[�v��
	fopen_s(&fp, path, "wb");
	
	if (fp)	//�t�@�C�������������ꍇ
	{
		for (auto list : namelist) //�I�u�W�F�N�g�̐����J��Ԃ�
		{
			fwrite(list.data(), list.size(), 1, fp);	//��������
			fwrite("/", 1, 1, fp);						//��؂�
		}
		//�t�@�C���N���[�Y
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
* @note       �f�[�^�����[�h����
* @attention  
******************************************************************************/
void SceneBase::DataLoad(std::list<std::string>& namelist)
{
	/*
	data/save/�V�[����.totsuka�̃t�@�C�����������A���[�h����
	�I�u�W�F�N�g��/�I�u�W�F�N�g��/�I�u�W�F�N�g��......�ƂȂ��Ă���̂ŁA��������namelist�ɓ����
	���̌�Adata/save/namelist.totsuka �̃f�[�^���Q�Ƃ��A�S�ẴI�u�W�F�N�g�����[�h����
	*/

	char path[256] = "data/save/";
	strcat(path, "Scene");
	strcat(path, ".totsuka"); //  data/save/�V�[����.totsuka

	FILE* fp;

	char* list = nullptr;

	//�t�@�C���I�[�v��
	fopen_s(&fp, path, "rb");
	
	if (fp) //�t�@�C�������������ꍇ
	{
		//�f�[�^�T�C�Y�擾
		fseek(fp, 0, SEEK_END); //�����Ɉړ�
		long Size = ftell(fp);  //�T�C�Y�擾
		list = new char[Size];  //�T�C�Y���̃��������m��
		fseek(fp, 0, SEEK_SET); //���̌�擪�ֈړ�

		//�ǂݍ���
		fread(list, Size, 1, fp);
		
		//�t�@�C���N���[�Y
		fclose(fp);

		// �u/�v�ŕ���
		Split('/', list, namelist);

	}
	else //�t�@�C����������Ȃ������ꍇ
		return;		//�I��

	for (auto name : namelist)  //�I�u�W�F�N�g�̐����J��Ԃ�
	{
		//�I�u�W�F�N�g�x�[�X�Ƀ��[�h
		ObjectBase save;

		char path[256] = "data/save/";
		strcat(path, name.data());
		strcat(path, ".totsuka");		//data/save/namelist.totsuka

		//�t�@�C���I�[�v��
		fopen_s(&fp, path, "rb");
		
		if (fp) //�t�@�C�������������ꍇ
		{
			//�I�u�W�F�N�g�����[�h
			fread(&save, sizeof(ObjectBase), 1, fp);

			//�t�@�C���N���[�Y
			fclose(fp);

			switch (save.GetType())	//�I�u�W�F�N�g�^�C�v�ʂɐ���
			{
			case BOX:	//�{�b�N�X
				Create<Box>(name.data());	//����
				GetComponent<Box>(name.data())->LoadFile(save);		//�ǂݍ��񂾃f�[�^���Z�b�g
				GetComponent<Box>(name.data())->Init(XMFLOAT3(1.0f,1.0f,1.0f));	//������(size 1.0f)
				break;
			case SPHERE: //�X�t�B�A(������)
				break;
			case FBX:  //FBX���f��
				Create<Model>(name.data());	//����
				GetComponent<Model>(name.data())->LoadFile(save);	//�ǂݍ��񂾃f�[�^���Z�b�g
				GetComponent<Model>(name.data())->Init();			//������
				break;
			}
		}
	}
}