//=============================================================================
//
// メッシュ クラス [Mesh.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================

#include "Mesh.h"
#include "backbuffer.h"
#include "Camera.h"
#include "light.h"

//定数定義
#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,0.7f)
#define M_SPECULAR		XMFLOAT4(0.1f,0.1f,0.1f,1.0f)
#define M_AMBIENT		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

HRESULT CMesh::Init(VERTEX_3D Vertex[], int nVertex, int* Index, int nIndex)
{
	

	BackBuffer* buffer = BackBuffer::GetBuffer();
	ID3D11Device* device = buffer->GetDevice();

	m_pConstantBuffer[0]->Create(sizeof(CONSTANT_BUFFER));
	m_pConstantBuffer[1]->Create(sizeof(CONSTANT_BUFFER2));

	// マテリアルの初期設定
	m_Material.Diffuse = M_DIFFUSE;
	m_Material.Ambient = M_AMBIENT;
	m_Material.Specular = M_SPECULAR;
	m_Material.Power = 0.0f;
	m_Material.Emissive = M_EMISSIVE;

	m_nIndex = nIndex;

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
		buffer->GetVertexShader()->GetData(),	// レイアウトと関連付ける頂点シェーダのデータ
		buffer->GetVertexShader()->GetSize(),	// レイアウトと関連付ける頂点シェーダのサイズ
		&m_InputLayout);			// 作成された頂点レイアウトの格納先

	if (FAILED(hr))
		return hr;

	//頂点バッファ作成
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(VERTEX_3D) * nVertex;	// バッファのサイズ
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// 使用方法
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND設定
	buffer_desc.CPUAccessFlags = 0;						// リソースへのCPUのアクセス権限についての設定
	buffer_desc.MiscFlags = 0;							// リソースオプションのフラグ
	buffer_desc.StructureByteStride = 0;				// 構造体のサイズ

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = Vertex;	// バッファの中身の設定
	sub_resource.SysMemPitch = 0;			// textureデータを使用する際に使用するメンバ
	sub_resource.SysMemSlicePitch = 0;		// textureデータを使用する際に使用するメンバ

	// バッファ作成
	if (FAILED(device->CreateBuffer(
		&buffer_desc,								// バッファ情報
		&sub_resource,								// リソース情報
		&m_VertexBuffers)))	// 作成されたバッファの格納先
	{
		return false;
	}

	//頂点バッファ作成
	buffer_desc.ByteWidth = (UINT)sizeof(UINT) * nIndex;	// バッファのサイズ
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// 使用方法
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND設定
	buffer_desc.CPUAccessFlags = 0;										// リソースへのCPUのアクセス権限についての設定
	buffer_desc.MiscFlags = 0;											// リソースオプションのフラグ
	buffer_desc.StructureByteStride = 0;								// 構造体のサイズ

	sub_resource.pSysMem = Index;							// バッファの中身の設定
	sub_resource.SysMemPitch = 0;										// textureデータを使用する際に使用するメンバ
	sub_resource.SysMemSlicePitch = 0;									// textureデータを使用する際に使用するメンバ

	// バッファ作成
	if (FAILED(device->CreateBuffer(
		&buffer_desc,						// バッファ情報
		&sub_resource,						// リソース情報
		&m_IndexBuffers)))		// 作成されたバッファの格納先
	{
		return false;
	}

	return hr;
}

void CMesh::Update()
{
	// ワールドマトリクス設定
	XMMATRIX translate = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMMATRIX rotate_x = XMMatrixRotationX(XMConvertToRadians(m_vDegree.x));
	XMMATRIX rotate_y = XMMatrixRotationY(XMConvertToRadians(m_vDegree.y));
	XMMATRIX rotate_z = XMMatrixRotationZ(XMConvertToRadians(m_vDegree.z));
	XMMATRIX scale_mat = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	m_mtxWorld = scale_mat * rotate_x * rotate_y * rotate_z * translate;

}

void CMesh::Draw(XMMATRIX& mtxWorld, VSShaderType vstype, PSShaderType pstype)
{
	BackBuffer *buffer = BackBuffer::GetBuffer();
	buffer->SetUpContext(vstype,pstype);
	UINT strides = sizeof(VERTEX_3D);
	UINT offsets = 0;

	//コンスタントバッファ
	CONSTANT_BUFFER cb;
	//カメラ情報
	CCamera* pCamera = CCamera::Get();

	// ワールドマトリクスをコンスタントバッファに設定
	cb.mWVP = XMMatrixTranspose(mtxWorld * XMLoadFloat4x4(&pCamera->GetViewMatrix()) * XMLoadFloat4x4(&pCamera->GetProjMatrix()));
	cb.mW = XMMatrixTranspose(mtxWorld);
	XMVECTOR light = DirectX::XMVector3Normalize(DirectX::XMVectorSet(CLight::Get()->GetDir().x, CLight::Get()->GetDir().y, CLight::Get()->GetDir().z, 0.0f));
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

	// インデックスバッファの数 = マテリアルの数だけメッシュを描画する
	// IA(InputAssemblerStage)に入力レイアウトを設定する
	buffer->GetDeviceContext()->IASetInputLayout(m_InputLayout);
	// IAに設定する頂点バッファの指定
	buffer->GetDeviceContext()->IASetVertexBuffers(
		0,						// バッファ送信のスロット番号
		1,						// バッファの数
		&m_VertexBuffers,		// 頂点バッファ
		&strides,				// バッファに使用している構造体のサイズ
		&offsets);				// 開始オフセット

	buffer->GetDeviceContext()->IASetIndexBuffer(
		m_IndexBuffers,
		DXGI_FORMAT_R32_UINT,
		0);

	// コンテキストにコンスタントバッファを設定
	m_pConstantBuffer[0]->SetVertexShader();
	m_pConstantBuffer[1]->SetPixelShader();

	// 描画
	buffer->GetDeviceContext()->DrawIndexed(
		m_nIndex,		// 頂点数
		0,						// オフセット
		0);						// 開始頂点のインデックス
	
}