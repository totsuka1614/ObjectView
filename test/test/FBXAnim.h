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

// マッピング情報
enum Mapping
{
	MAPPING_VERTEX,
	MAPPING_INDEX,
};


// メッシュ情報
struct MeshInfo
{
	// 法線情報
	struct NormalInfo
	{
		typedef std::vector<XMFLOAT3> List;
		Mapping	mapping;
		List	list;
	};
	// UV情報
	struct UVInfo
	{
		typedef std::vector<XMFLOAT2> List;
		Mapping	mapping;
		List	list;
	};
	typedef std::vector<UVInfo> UVList;
	// スキン
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

	IndexList		indexList;		///< インデックス情報
	VertexList		vertexList;		///< 頂点情報
	NormalInfo		normalInfo;		///< 法線情報
	UVList			uvList;			///< UV情報
	MaterialName	materialName;	///< マテリアル名(マテリアルで分割してるので一つ
	SkinInfo		skinInfo;		///< スキン情報
	Matrix			transform;		///< 姿勢情報
};
typedef std::vector<MeshInfo> MeshList;

struct FBXVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
	// メッシュとボーンを結びつけるためのデータ
	float weight[4]; // それぞれの骨からどの程度影響を受けるか
	long index[4]; // どの骨に引っ付いて動くか
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