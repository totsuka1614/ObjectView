#pragma once

#include "main.h"
#include <vector>
#include <string>

// 回転順序
enum RotateOrder
{
	ROTATE_XYZ,
	ROTATE_ZXY,
};

// ボーン情報
struct FbxBoneInfo
{
	typedef std::vector<long> ChildList;

	std::string		name;		///< ボーン名
	XMFLOAT3		translate;	///< 移動値
	XMFLOAT3		rotation;	///< 回転値
	XMFLOAT3		scale;		///< 拡大縮小
	XMFLOAT3		preRotate;	///< 事前回転
	RotateOrder		order;		///< 回転順序
	ChildList		children;	///< 子要素

};
typedef std::vector<FbxBoneInfo> BoneList;

// ボーン一覧
struct BoneListInfo
{
	int root;
	BoneList list;
};

// アニメーション情報
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

		std::string		name;					///< 対応するボーン名
		List			keyList[CHANNEL_MAX];	///< チャンネルごとのキー情報
	};
	typedef std::vector<CurveInfo> List;

	long		totalFrame;		///< アニメーション時間
	List		curveList;		///< アニメ情報
};

struct MeshInverse
{
	struct List
	{
		int boneIndex;
		// もともとのモデルはTポーズを取っている
		// そのTポーズの移動情報を打ち消し、新たにアニメーションの
		// 行列を適用することで、頂点が正しいアニメーションの位置に
		// 移動する
		XMMATRIX invMat; // Tポーズの移動を打ち消す逆行列
	};
	List* pList;
	int num;
};

struct BoneInfo
{
	XMFLOAT4X4 matrix; // 骨の姿勢
	XMMATRIX preRotate; // あらかじめ回転している量
	MatrixParam param; // デフォルトの骨の姿勢
};

struct AnimeInfo
{
	float playFrame; // 現在再生中のフレーム
	bool loop; // 繰り返し再生するかどうか
	float speed; // アニメーションの再生スピード
	AnimationInfo* pAnime; // アニメの情報
};