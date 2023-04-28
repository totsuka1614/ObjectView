/******************************************************************************
* 
* @file      FBX.h
* @brief     FBXFileクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      FBX SDKを利用
* @attention 
* 
******************************************************************************/
#ifndef __FBX_H__
#define __FBX_H__
//インクルード部
#include <fbxsdk.h>
#include <vector>
#include <map>
#include "main.h"
#include "vertex.h"
#include "ObjectBase.h"
#include "backbuffer.h"
#include "ConstantBuffer.h"

//Attributeとは：ノード情報が格納されている
//	eMesh：メッシュ
//	eSkeleton：ボーン
//	eCamera：カメラ
//	eLight：ライト
//	eUnknown：不明

class CFBXFile : public CObjectBase
{
public:
	//コンストラクタ
	CFBXFile():m_InputLayout(nullptr)
	{
		m_eType = FBX;
		m_VertexBuffers.clear();
		m_IndexBuffers.clear();
		m_Textures.clear();
		m_Vertices.clear();
		m_Indices.clear();
		m_pConstantBuffer[0] = new CConstantBuffer;
		m_pConstantBuffer[1] = new CConstantBuffer;
		m_pBoneBuffer = new CConstantBuffer;

		m_mtxWorld = XMMatrixIdentity();
	}
	//デストラクタ
	~CFBXFile()
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
	HRESULT LoadTex(FbxFileTexture* material, std::string& keyword);
	void LoadMat(FbxSurfaceMaterial*);
	void LoadUV(const char* node_name, FbxMesh* mesh);
	void SetMaterialName(FbxMesh* mesh);
	//描画関数
	void Draw();
	void EdgeDraw();
	void SetMaterial(XMFLOAT4& dif) { m_Material.Diffuse = dif; }
	void SetWorldMatrix(XMMATRIX mW) { m_mtxWorld = mW; }
	
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
	
	HRESULT CreateInputLayout(ID3D11Device* device, CVertex* vertex_shader);

	//メッシュ作成関数
	bool CreateMesh(const char* node_name, FbxMesh* mesh);

	//メッシュを探索し格納
	void CollectMeshNode(FbxNode* node, std::map<std::string, FbxNode*>& list);

	//mapクラスについて
	//↳名前,データという紐づけができる
	//参考:https://cpp-lang.sevendays-study.com/ex-day5.html

	std::map<std::string, ID3D11Buffer*> m_VertexBuffers;			//頂点バッファ
	std::map<std::string, ID3D11Buffer*> m_IndexBuffers;			//インデックスバッファ
	std::map<std::string, ID3D11ShaderResourceView*> m_Textures;	//テクスチャ
	ID3D11InputLayout* m_InputLayout;								//入力レイアウト
	CConstantBuffer* m_pConstantBuffer[2];							//定数バッファ 0:頂点 1:ピクセル
	CConstantBuffer* m_pBoneBuffer;							//定数バッファ 0:頂点 1:ピクセル
	std::map<std::string, std::vector<VERTEX_3D>> m_Vertices;		//頂点バッファ
	std::map<std::string, std::vector<UINT>> m_Indices;				//インデックスバッファ
	std::vector<std::string> m_MaterialName;
	std::vector<std::string> m_texture_name;
	int m_meshNum;
};
#endif // !__FBX_H__