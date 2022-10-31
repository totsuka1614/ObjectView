//=============================================================================
//
// FBXFile クラス [FBXFile.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "FBX.h"
#include "backbuffer.h"
#include "Camera.h"
#include "light.h"
#include <codecvt> 
#include <vector>
#include <DirectXTex.h>
#include "WICTextureLoader.h"
#include "Utility.h"
#include "Texture.h"

#pragma comment(lib, "DirectXTex.lib")

//定数定義
#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.1f,0.1f,0.1f,1.0f)
#define M_AMBIENT		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

std::vector<std::string> g_texture_name;

HRESULT FBXFile::Load(const char* file_name)
{
	BackBuffer* buffer = BackBuffer::GetBuffer();
	
	m_pConstantBuffer[0]->Create(sizeof(CONSTANT_BUFFER));
	m_pConstantBuffer[1]->Create(sizeof(CONSTANT_BUFFER2));

	// マテリアルの初期設定
	m_Material.Diffuse = M_DIFFUSE;
	m_Material.Ambient = M_AMBIENT;
	m_Material.Specular = M_SPECULAR;
	m_Material.Power = 0.0f;
	m_Material.Emissive = M_EMISSIVE;

	if (LoadFbxFile(file_name) == false)
	{
		return false;
	}

	if (CreateVertexBuffer(buffer->GetDevice()) == false)
	{
		return false;
	}

	if (CreateIndexBuffer(buffer->GetDevice()) == false)
	{
		return false;
	}

	if (CreateInputLayout(buffer->GetDevice(), buffer->GetVertexShader()) == false)
	{
		return false;
	}

	return true;
}

bool FBXFile::LoadFbxFile(const char* file_name)
{
	// FbxManager作成
	//FbxManagerとは
	//↳ImpoterやFBXSceneを作成するための管理クラス
	//参考 https://yttm-work.jp/model_render/model_render_0007.html
	FbxManager* fbx_manager = fbxsdk::FbxManager::Create();
	if (fbx_manager == nullptr)
	{
		return false;
	}

	// FbxImporter作成
	//FbxImporterとは
	//↳Fbxをインポートするためのパーサークラス
	FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "");
	if (fbx_importer == nullptr)
	{
		fbx_manager->Destroy();
		return false;
	}

	// FbxScene作成
	//FbxSceneとは
	//↳FbxImporterで取得した情報を格納するクラス
	FbxScene* fbx_scene = FbxScene::Create(fbx_manager, "");
	if (fbx_scene == nullptr)
	{
		fbx_importer->Destroy();
		fbx_manager->Destroy();
		return false;
	}

	// Fileを初期化
	fbx_importer->Initialize(file_name);
	// sceneにインポート
	fbx_importer->Import(fbx_scene);

	FbxGeometryConverter converter(fbx_manager);
	// ポリゴンを三角形にする
	converter.Triangulate(fbx_scene, true);

	//ノードマップクラス作成
	std::map<std::string, FbxNode*> mesh_node_list;
	// メッシュNodeを探す
	CollectMeshNode(fbx_scene->GetRootNode(), mesh_node_list);

	int material_num = fbx_scene->GetSrcObjectCount<FbxSurfaceMaterial>();

	for (int i = 0; i < material_num; i++)
	{
		LoadMat(fbx_scene->GetSrcObject<FbxSurfaceMaterial>(i));
	}

	//イテレータ作成
	std::map<std::string, FbxNode*>::iterator it = mesh_node_list.begin();

	while (it != mesh_node_list.end())
	{
		// mesh作成
		CreateMesh(it->first.c_str(), it->second->GetMesh());

		it++;
	}

	fbx_importer->Destroy();
	fbx_scene->Destroy();
	fbx_manager->Destroy();

	return true;
}

void FBXFile::CollectMeshNode(FbxNode* node, std::map<std::string, FbxNode*>& list)
{
	
	for (int i = 0; i < node->GetNodeAttributeCount(); i++) {	//ノードの数だけ回す

		//Attribute取得
		FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);

		//Attributeのタイプを取得
		FbxNodeAttribute::EType type = attribute->GetAttributeType();

		//typeがメッシュだったら
		if (type == FbxNodeAttribute::EType::eMesh)
		{
			list[node->GetName()] = node;
			break;
		}
	}

	//子ノードがある分だけ回す
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		CollectMeshNode(node->GetChild(i), list);
	}
}

bool FBXFile::CreateMesh(const char* node_name, FbxMesh* mesh)
{
	// 頂点座標リスト取得
	FbxVector4* vertices = mesh->GetControlPoints();
	// インデックスバッファの取得
	int* indices = mesh->GetPolygonVertices();
	// 頂点座標の数の取得
	int polygon_vertex_count = mesh->GetPolygonVertexCount();

	for (int i = 0; i < polygon_vertex_count; i++)	//頂点の数だけ回す
	{
		VERTEX_3D vertex;
		// インデックスバッファから頂点番号を取得
		int index = indices[i];

		// 頂点座標リストから座標を取得する
		vertex.vtx.x = (float)-vertices[index][0];	//右手座標系から左手座標系にするためにマイナスに
		vertex.vtx.y = (float)vertices[index][1];
		vertex.vtx.z = (float)vertices[index][2];

		// 追加
		m_Vertices[node_name].push_back(vertex);
	}

	FbxArray<FbxVector4> normals;
	// 法線リストの取得
	mesh->GetPolygonVertexNormals(normals);

	// 法線設定
	for (int i = 0; i < normals.Size(); i++)
	{
		// 法線座標リストから法線を取得する
		m_Vertices[node_name][i].nor.x = (float)-normals[i][0];//右手座標系から左手座標系にするためにマイナスに
		m_Vertices[node_name][i].nor.y = (float)normals[i][1];
		m_Vertices[node_name][i].nor.z = (float)normals[i][2];
	}

	// ポリゴン数の取得
	int polygon_count = mesh->GetPolygonCount();

	// ポリゴンの数だけ連番として保存する
	for (int i = 0; i < polygon_count; i++)
	{
		//右手系座標にするため
		m_Indices[node_name].push_back(i * 3 + 2);
		m_Indices[node_name].push_back(i * 3 + 1);
		m_Indices[node_name].push_back(i * 3);
	}

	LoadUV(node_name,mesh);

	return true;
}

void FBXFile::LoadUV(const char* node_name,FbxMesh* mesh)
{
	FbxStringList uvset_names;
	// UVSetの名前リストを取得
	mesh->GetUVSetNames(uvset_names);

	FbxArray<FbxVector2> uv_buffer;

	// UVSetの名前からUVSetを取得する
	// 今回はマルチテクスチャには対応しないので最初の名前を使う

	mesh->GetPolygonVertexUVs(uvset_names.GetStringAt(0), uv_buffer);
	for (int i = 0; i < uv_buffer.Size(); i++)
	{
		FbxVector2& uv = uv_buffer[i];

		// そのまま使用して問題ない
		m_Vertices[node_name][i].tex.x = (float)uv[0];
		m_Vertices[node_name][i].tex.y = (float)(1.0 - uv[1]);
	}
}

void FBXFile::LoadMat(FbxSurfaceMaterial* material)
{
	enum class MaterialOrder
	{
		Ambient,
		Diffuse,
		Specular,
		MaxOrder,
	};

	FbxDouble3 colors[(int)MaterialOrder::MaxOrder];
	FbxDouble factors[(int)MaterialOrder::MaxOrder];
	FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sAmbient);
	if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		const char* element_check_list[] =
		{
			FbxSurfaceMaterial::sAmbient,
			FbxSurfaceMaterial::sDiffuse,
		};

		const char* factor_check_list[] =
		{
			FbxSurfaceMaterial::sAmbientFactor,
			FbxSurfaceMaterial::sDiffuseFactor,
		};

		for (int i = 0; i < 2; i++)
		{
			prop = material->FindProperty(element_check_list[i]);
			if (prop.IsValid())
			{
				colors[i] = prop.Get<FbxDouble3>();

			}
			else
			{
				colors[i] = FbxDouble3(1.0, 1.0, 1.0);
			}

			prop = material->FindProperty(factor_check_list[i]);
			if (prop.IsValid())
			{
				factors[i] = prop.Get<FbxDouble>();
			}
			else
			{
				factors[i] = 1.0;
			}
		}
	}

	FbxDouble3 color = colors[(int)MaterialOrder::Ambient];
	FbxDouble factor = factors[(int)MaterialOrder::Ambient];
	m_Material.Ambient = XMFLOAT4((float)color[0], (float)color[1], (float)color[2], (float)factor);
	
	color = colors[(int)MaterialOrder::Diffuse];
	factor = factors[(int)MaterialOrder::Diffuse];
	m_Material.Diffuse = XMFLOAT4((float)color[0], (float)color[1], (float)color[2], (float)factor);

		// テクスチャ読み込み(シングル対応)
	// Diffuseプロパティを取得
	prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	FbxFileTexture* texture = nullptr;
	std::string keyword;
	int texture_num = prop.GetSrcObjectCount<FbxFileTexture>();
	if (texture_num > 0)
	{
		// propからFbxFileTextureを取得	
		texture = prop.GetSrcObject<FbxFileTexture>(0);
	}
	else
	{
		// FbxLayeredTextureからFbxFileTextureを取得	
		int layer_num = prop.GetSrcObjectCount<FbxLayeredTexture>();
		if (layer_num > 0)
		{
			texture = prop.GetSrcObject<FbxFileTexture>(0);
		}
	}

	if (texture != nullptr &&

		LoadTex(texture, keyword) == true)
	{
	}
}

HRESULT FBXFile::LoadTex(FbxFileTexture* texture, std::string& keyword)
{
	ID3D11Device* pDevice = BackBuffer::GetBuffer()->GetDevice();

	if (texture == nullptr)
	{	
		return false;
	}

	// ファイル名を取得	
	std::string file_path = texture->GetRelativeFileName();

	// ファイル分解
	char buffer[256];
	ZeroMemory(buffer, sizeof(char) * 256);
	memcpy(buffer, file_path.c_str(), sizeof(char) * 256);

	std::vector<std::string> split_list;
	
	// 記号統一
	Replace('\\', '/', buffer);

	// 「/」で分解　必要なし
	Split('/', buffer, split_list);

	std::string root_path = "data/Texture/";
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::wstring wstr_file_name = cv.from_bytes(root_path + split_list[split_list.size() - 1]);


	// 文字化け対策
	char* file_name;
	size_t size = 0;
	FbxAnsiToUTF8(split_list[split_list.size() - 1].c_str(), file_name, &size);

	if (m_Textures.count(file_name) > 0 && m_Textures[file_name] != nullptr)
	{
		FbxFree(file_name);
		return true;
	}

	HRESULT hr;
	hr = CreateTextureFromFile(pDevice, wstr_file_name.c_str(),&m_Textures[file_name]);
	
	keyword = file_name;

	g_texture_name.push_back(keyword);
		
	FbxFree(file_name);
	return true;
}

bool FBXFile::CreateVertexBuffer(ID3D11Device* device)
{
	for (std::pair<const std::string, std::vector<VERTEX_3D>> vertex : m_Vertices)
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(VERTEX_3D) * vertex.second.size();	// バッファのサイズ
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// 使用方法
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND設定
		buffer_desc.CPUAccessFlags = 0;						// リソースへのCPUのアクセス権限についての設定
		buffer_desc.MiscFlags = 0;							// リソースオプションのフラグ
		buffer_desc.StructureByteStride = 0;				// 構造体のサイズ

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &vertex.second[0];	// バッファの中身の設定
		sub_resource.SysMemPitch = 0;			// textureデータを使用する際に使用するメンバ
		sub_resource.SysMemSlicePitch = 0;		// textureデータを使用する際に使用するメンバ

		// バッファ作成
		if (FAILED(device->CreateBuffer(
			&buffer_desc,								// バッファ情報
			&sub_resource,								// リソース情報
			&m_VertexBuffers[vertex.first])))	// 作成されたバッファの格納先
		{
			return false;
		}
	}

	return true;
}

bool FBXFile::CreateIndexBuffer(ID3D11Device* device)
{
	for (std::pair<const std::string, std::vector<UINT>> index : m_Indices)
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = (UINT)sizeof(UINT) * index.second.size();	// バッファのサイズ
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// 使用方法
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND設定
		buffer_desc.CPUAccessFlags = 0;										// リソースへのCPUのアクセス権限についての設定
		buffer_desc.MiscFlags = 0;											// リソースオプションのフラグ
		buffer_desc.StructureByteStride = 0;								// 構造体のサイズ

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &index.second[0];							// バッファの中身の設定
		sub_resource.SysMemPitch = 0;										// textureデータを使用する際に使用するメンバ
		sub_resource.SysMemSlicePitch = 0;									// textureデータを使用する際に使用するメンバ

		// バッファ作成
		if (FAILED(device->CreateBuffer(
			&buffer_desc,						// バッファ情報
			&sub_resource,						// リソース情報
			&m_IndexBuffers[index.first])))		// 作成されたバッファの格納先
		{
			return false;
		}
	}

	return true;
}

HRESULT FBXFile::CreateInputLayout(ID3D11Device* device, Vertex* vertex_shader)
{
	D3D11_INPUT_ELEMENT_DESC vertex_desc[]{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	HRESULT hr;

	//頂点レイアウト作成
	hr = device->CreateInputLayout(
		vertex_desc,				// レイアウト設定
		ARRAYSIZE(vertex_desc),		// 配列サイズ
		vertex_shader->GetData(),	// レイアウトと関連付ける頂点シェーダのデータ
		vertex_shader->GetSize(),	// レイアウトと関連付ける頂点シェーダのサイズ
		&m_InputLayout);			// 作成された頂点レイアウトの格納先

	if (FAILED(hr))
		return hr;

	return hr;
}

void FBXFile::Draw(XMMATRIX& mtxWorld ,PSShaderType type)
{

	BackBuffer *buffer = BackBuffer::GetBuffer();
	buffer->SetUpContext(VERTEX,type);
	UINT strides = sizeof(VERTEX_3D);
	UINT offsets = 0;

	//コンスタントバッファ
	CONSTANT_BUFFER cb;
	//カメラ情報
	CCamera* pCamera = CCamera::Get();

	// ワールドマトリクスをコンスタントバッファに設定
	cb.mWVP = XMMatrixTranspose(mtxWorld * XMLoadFloat4x4(&pCamera->GetViewMatrix()) * XMLoadFloat4x4(&pCamera->GetProjMatrix()));
	cb.mW = XMMatrixTranspose(mtxWorld);
	XMVECTOR light = DirectX::XMVector3Normalize(DirectX::XMVectorSet(CLight::Get()->GetDir().x, CLight::Get()->GetDir().y, CLight::Get()->GetDir().z,0.0f));
	XMStoreFloat4(&cb.fLightVector, light);
	// コンスタントバッファ更新
	m_pConstantBuffer[0]->Update(&cb);

	CONSTANT_BUFFER2 cb2;
	CLight* pLight = CLight::Get();
	cb2.vEye = XMLoadFloat3(&pCamera->GetTransform());
	cb2.vLightDir = XMLoadFloat3(&pLight->GetDir());
	cb2.vLa = XMLoadFloat4(&pLight->GetAmbient());
	cb2.vLd = XMLoadFloat4(&pLight->GetDiffuse());
	cb2.vLs = XMLoadFloat4(&pLight->GetSpecular());
	cb2.vDiffuse = XMLoadFloat4(&m_Material.Diffuse);
	cb2.vAmbient = XMVectorSet(m_Material.Ambient.x, m_Material.Ambient.y, m_Material.Ambient.z, 0.f);
	cb2.vSpecular = XMVectorSet(m_Material.Specular.x, m_Material.Specular.y, m_Material.Specular.z, m_Material.Power);
	cb2.vEmissive = XMLoadFloat4(&m_Material.Emissive);
	m_pConstantBuffer[1]->Update(&cb2);
	
	int f = 0;
	auto it = g_texture_name.begin();

	for (std::pair<const std::string, std::vector<UINT>> index : m_Indices)
	{
		// インデックスバッファの数 = マテリアルの数だけメッシュを描画する
		// IA(InputAssemblerStage)に入力レイアウトを設定する
		buffer->GetDeviceContext()->IASetInputLayout(m_InputLayout);
		// IAに設定する頂点バッファの指定
		buffer->GetDeviceContext()->IASetVertexBuffers(
			0,						// バッファ送信のスロット番号
			1,						// バッファの数
			&m_VertexBuffers[index.first],		// 頂点バッファ
			&strides,				// バッファに使用している構造体のサイズ
			&offsets);				// 開始オフセット

		buffer->GetDeviceContext()->IASetIndexBuffer(
			m_IndexBuffers[index.first],
			DXGI_FORMAT_R32_UINT,
			0);

		// コンテキストにコンスタントバッファを設定
		m_pConstantBuffer[0]->SetVertexShader();
		m_pConstantBuffer[1]->SetPixelShader();


		if (it != g_texture_name.end())
		{
			buffer->SetTexture(m_Textures[g_texture_name[f].c_str()]);
			it++;
		}

		f++;
	
		// 描画
		buffer->GetDeviceContext()->DrawIndexed(
			index.second.size(),		// 頂点数
			0,						// オフセット
			0);						// 開始頂点のインデックス
	}
}

