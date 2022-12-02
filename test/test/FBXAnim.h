#pragma once

#include "main.h"
#include <vector>
#include <string>

// ��]����
enum RotateOrder
{
	ROTATE_XYZ,
	ROTATE_ZXY,
};

// �{�[�����
struct FbxBoneInfo
{
	typedef std::vector<long> ChildList;

	std::string		name;		///< �{�[����
	XMFLOAT3		translate;	///< �ړ��l
	XMFLOAT3		rotation;	///< ��]�l
	XMFLOAT3		scale;		///< �g��k��
	XMFLOAT3		preRotate;	///< ���O��]
	RotateOrder		order;		///< ��]����
	ChildList		children;	///< �q�v�f

};
typedef std::vector<FbxBoneInfo> BoneList;

// �{�[���ꗗ
struct BoneListInfo
{
	int root;
	BoneList list;
};

// �A�j���[�V�������
struct AnimationInfo
{
	struct CurveInfo
	{
		enum Channels
		{
			TRANSLATION_X,
			TRANSLATION_Y,
			TRANSLATION_Z,
			ROTATION_X,
			ROTATION_Y,
			ROTATION_Z,
			SCALING_X,
			SCALING_Y,
			SCALING_Z,
			CHANNEL_MAX
		};
		struct KeyInfo
		{
			long	frame;
			float	value;
		};
		typedef std::vector<KeyInfo> List;

		std::string		name;					///< �Ή�����{�[����
		List			keyList[CHANNEL_MAX];	///< �`�����l�����Ƃ̃L�[���
	};
	typedef std::vector<CurveInfo> List;

	long		totalFrame;		///< �A�j���[�V��������
	List		curveList;		///< �A�j�����
};

struct MeshInverse
{
	struct List
	{
		int boneIndex;
		// ���Ƃ��Ƃ̃��f����T�|�[�Y������Ă���
		// ����T�|�[�Y�̈ړ�����ł������A�V���ɃA�j���[�V������
		// �s���K�p���邱�ƂŁA���_���������A�j���[�V�����̈ʒu��
		// �ړ�����
		XMMATRIX invMat; // T�|�[�Y�̈ړ���ł������t�s��
	};
	List* pList;
	int num;
};

struct BoneInfo
{
	XMFLOAT4X4 matrix; // ���̎p��
	XMMATRIX preRotate; // ���炩���߉�]���Ă����
	MatrixParam param; // �f�t�H���g�̍��̎p��
};

struct AnimeInfo
{
	float playFrame; // ���ݍĐ����̃t���[��
	bool loop; // �J��Ԃ��Đ����邩�ǂ���
	float speed; // �A�j���[�V�����̍Đ��X�s�[�h
	AnimationInfo* pAnime; // �A�j���̏��
};