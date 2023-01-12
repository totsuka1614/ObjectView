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

union Matrix
{
	struct
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
	};
	float m[4][4];
	float a[16];
};
struct Color
{
	float r, g, b, a;
};

// �}�b�s���O���
enum Mapping
{
	MAPPING_VERTEX,
	MAPPING_INDEX,
};


// ���b�V�����
struct MeshInfo
{
	// �@�����
	struct NormalInfo
	{
		typedef std::vector<XMFLOAT3> List;
		Mapping	mapping;
		List	list;
	};
	// UV���
	struct UVInfo
	{
		typedef std::vector<XMFLOAT2> List;
		Mapping	mapping;
		List	list;
	};
	typedef std::vector<UVInfo> UVList;
	// �X�L��
	struct SkinInfo
	{
		struct WeightInfo
		{
			struct WeightValue
			{
				long index;
				float weight;
			};
			static const long WEIGHT_NUM_MAX = 4;
			WeightValue value[WEIGHT_NUM_MAX];
		};
		struct BoneInfo
		{
			std::string name;
			float offset[4][4];
		};
		typedef std::vector<WeightInfo> WeightList;
		typedef std::vector<BoneInfo> BoneList;

		WeightList		weightList;
		BoneList		boneList;
	};

	typedef std::vector<long> IndexList;
	typedef std::vector<XMFLOAT3> VertexList;
	typedef std::string MaterialName;

	IndexList		indexList;		///< �C���f�b�N�X���
	VertexList		vertexList;		///< ���_���
	NormalInfo		normalInfo;		///< �@�����
	UVList			uvList;			///< UV���
	MaterialName	materialName;	///< �}�e���A����(�}�e���A���ŕ������Ă�̂ň��
	SkinInfo		skinInfo;		///< �X�L�����
	Matrix			transform;		///< �p�����
};
typedef std::vector<MeshInfo> MeshList;

struct FBXVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
	// ���b�V���ƃ{�[�������т��邽�߂̃f�[�^
	float weight[4]; // ���ꂼ��̍�����ǂ̒��x�e�����󂯂邩
	long index[4]; // �ǂ̍��Ɉ����t���ē�����
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