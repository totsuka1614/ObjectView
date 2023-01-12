//=============================================================================
//
// FBXFile クラス定義 [FBX.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef __FBX_H__
#define __FBX_H__

#include <fbxsdk.h>
#include <vector>
#include <map>
#include "main.h"
#include "vertex.h"
#include "ObjectBase.h"
#include "backbuffer.h"
#include "ConstantBuffer.h"
#include "FBXAnim.h"

//Attributeとは：ノード情報が格納されている
//	eMesh：メッシュ
//	eSkeleton：ボーン
//	eCamera：カメラ
//	eLight：ライト
//	eUnknown：不明



class FBXFile : public ObjectBase
{
public:
	//コンストラクタ
	FBXFile():m_InputLayout(nullptr)
	{
		m_eType = FBX;
		m_VertexBuffers.clear();
		m_IndexBuffers.clear();
		m_Textures.clear();
		m_Vertices.clear();
		m_Indices.clear();
		m_pConstantBuffer[0] = new ConstantBuffer;
		m_pConstantBuffer[1] = new ConstantBuffer;
		m_pBoneBuffer = new ConstantBuffer;

		for (int i = 0; i < MaxAnime; ++i)
		{
			m_pAnimes[i] = nullptr;
		}
		for (int i = 0; i < MaxBlend; ++i)
		{
			m_pBlendBones[i] = nullptr;
		}
	}
	//デストラクタ
	~FBXFile()
	{
		m_VertexBuffers.clear();
		m_IndexBuffers.clear();
		m_Textures.clear();
		m_Vertices.clear();
		m_Indices.clear();
		SAFE_RELEASE(m_InputLayout);
		SAFE_DELETE(m_pConstantBuffer[0]);
		SAFE_DELETE(m_pConstantBuffer[1]);
	}

	//読み込み関数
	HRESULT Load(const char* file_name);
	void LoadAnim(const char* file_name);
	void LoadBone(void);
	void LoadUV(const char* node_name,FbxMesh*);
	void LoadMesh(void);
	HRESULT LoadTex(FbxFileTexture* material, std::string& keyword);
	void LoadMat(FbxSurfaceMaterial*);
	void SetMaterialName(FbxMesh* mesh);
	//描画関数
	void Draw();
	void EdgeDraw();
	void Step(float speed);
	void PlayAnime(int animeNo, bool isLoop);
	void PlayBlendAnime(int blendNo, float time, bool isLoop);
	void SetMaterial(XMFLOAT4& dif) { m_Material.Diffuse = dif; }
	XMFLOAT4X4 GetBone(int index)
	{
		if (index < 0 || m_pFBXBone->list.size() <= index)
		{
			return DirectX::XMFLOAT4X4(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);
		}

		return m_pBones[index].matrix;
	}
	std::vector<MeshInverse>& GetMeshInverse(){return m_meshInverse;}

private:

	//ファイル読み込み
	bool LoadFbxFile( const char* file_name = "Box.fbx");

	/**
	* @brief VertexBuffer作成@n
	* VertexBufferを作成する
	* @return 作成の成否 成功(true)
	* @param[in] device DirectX11グラフィクスデバイス
	*/
	bool CreateVertexBuffer(ID3D11Device* device);

	/**
	* @brief IndexBuffer作成@n
	* IndexBufferを作成する
	* @return 作成の成否 成功(true)
	* @param[in] device DirectX11グラフィクスデバイス
	*/
	bool CreateIndexBuffer(ID3D11Device* device);

		/**
	* @brief InputLayout作成@n
	* InputLayoutを作成する
	* @return 作成の成否 成功(true)
	* @param[in] device DirectX11グラフィクスデバイス
	* @param[in] vertex_shader InputLayoutと関連付ける頂点シェーダ
	*/
	
	HRESULT CreateInputLayout(ID3D11Device* device, Vertex* vertex_shader);

	//メッシュ作成関数
	bool CreateMesh(const char* node_name, FbxMesh* mesh);

	//メッシュを探索し格納
	void CollectMeshNode(FbxNode* node, std::map<std::string, FbxNode*>& list);

	//ボーン関係-------------------------------------------------------------------
	void CleanupBone(BoneListInfo &boneListInfo);
	void GetBone(FbxScene *pScene, BoneListInfo &boneListInfo, bool isMirror);
	int GetRootBone(FbxNode *pNode, BoneList &list);
	void CalcBone(int index, DirectX::XMMATRIX parent);
	//----------------------------------------------------------------------------

	//メッシュ関係------------------------------------------------------------------
	void CleanupMesh(MeshList& meshList);
	void GetMesh(FbxScene* pScene, MeshList& meshList, bool isMirror);
	void GetIndex(MeshInfo::IndexList &list, FbxMesh *pMesh, bool isMirror);
	void GetVertex(MeshInfo::VertexList &list, FbxMesh *pMesh, bool isMirror);
	void GetNormal(MeshInfo::NormalInfo &info, FbxMesh *pMesh, MeshInfo::IndexList &idxList, MeshInfo::VertexList &vtxList, bool isMirror);
	void GetUV(MeshInfo::UVList &list, FbxMesh *pMesh, MeshInfo::IndexList &idxList, MeshInfo::VertexList &vtxList, bool isMirror);
	void GetMaterial(MeshInfo::MaterialName &name, FbxMesh *pMesh, FbxNode *pNode);
	void GetSkin(MeshInfo::SkinInfo &info, FbxMesh *pMesh, MeshInfo::VertexList &vtxList, bool isMirror);
	void GetTransform(Matrix& mat, FbxMesh* pMesh, bool isMirror);
	//------------------------------------------------------------------------------

	//アニメーション関係-----------------------------------------------------------
	void CleanupAnimation(AnimationInfo &anime);
	void GetAnimation(FbxScene *pScene, AnimationInfo &anime, bool isMirror);
	void GetKeyInfos(FbxAnimCurve *pCurve, AnimationInfo::CurveInfo::List &list);
	void CalcAnimation(FbxAnimLayer* pLayer, FbxNode* pNode, AnimationInfo &anime, bool isMirror);
	void MirrorKeys(AnimationInfo::CurveInfo::List& list);
	void CalcAnimeFrame(int animeNo, float speed);
	void CalcAnime(int animeNo, MatrixParam* pBones);
	float CalcKey(AnimationInfo::CurveInfo::List& keys, float frame, float defValue);
	//----------------------------------------------------------------------------

	//mapクラスについて
	//↳名前,データという紐づけができる
	//参考:https://cpp-lang.sevendays-study.com/ex-day5.html

	std::map<std::string, ID3D11Buffer*> m_VertexBuffers;			//頂点バッファ
	std::map<std::string, ID3D11Buffer*> m_IndexBuffers;			//インデックスバッファ
	std::map<std::string, ID3D11ShaderResourceView*> m_Textures;	//テクスチャ
	ID3D11InputLayout* m_InputLayout;								//入力レイアウト
	ConstantBuffer* m_pConstantBuffer[2];							//定数バッファ 0:頂点 1:ピクセル
	ConstantBuffer* m_pBoneBuffer;							//定数バッファ 0:頂点 1:ピクセル
	std::map<std::string, std::vector<VERTEX_3D>> m_Vertices;		//頂点バッファ
	std::map<std::string, std::vector<UINT>> m_Indices;				//インデックスバッファ
	std::string m_MaterialName;
	std::vector<std::string> m_texture_name;
	std::vector<MeshInverse> m_meshInverse;
	int m_meshNum;
	//--- 骨の情報
	// 骨一本一本の位置や回転の情報、
	// どの骨がどこに繋がってるか、骨の名称が含まれる
	// また、どこから計算を始めるのかといった情報を含む
	BoneListInfo* m_pFBXBone; // FBXの骨情報
	BoneInfo* m_pBones; // プログラム実行中の骨の情報
	const static int MaxBlend = 2;
	MatrixParam* m_pBlendBones[MaxBlend]; // 遷移用の骨情報
	//--- アニメーションの情報
	const static int MaxAnime = 10;
	AnimeInfo* m_pAnimes[MaxAnime];
	int m_playAnimeNo; // 現在再生中のアニメ番号

	int m_blendAnimeNo; // 遷移中のアニメ番号
	float m_blendFrame; // 遷移時間
};
#endif // !__FBX_H__