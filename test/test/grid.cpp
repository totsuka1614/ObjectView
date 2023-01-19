#include "grid.h"
#include "Camera.h"
#include "light.h"
#include "Input.h"

//定数定義
#define M_DIFFUSE		XMFLOAT4(0.2f,0.2f,0.2f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.1f,0.1f,0.1f,1.0f)
#define M_AMBIENT		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define GRID_WIDTH (600.0f)
#define GRID_HEIGHT (600.0f)
#define GRID_COUNT (61)

void Grid::Init(void)
{
	m_pConstantBuffer[0] = new ConstantBuffer;
	m_pConstantBuffer[0]->Create(sizeof(CONSTANT_BUFFER));
	m_pConstantBuffer[1] = new ConstantBuffer;
	m_pConstantBuffer[1]->Create(sizeof(CONSTANT_BUFFER2));

	// マテリアルの初期設定
	m_Material.Diffuse = M_DIFFUSE;
	m_Material.Ambient = M_AMBIENT;
	m_Material.Specular = M_SPECULAR;
	m_Material.Power = 0.0f;
	m_Material.Emissive = M_EMISSIVE;

	Vertex vertex;
	
	vertex.pos[0] = -GRID_WIDTH * 2;
	vertex.pos[1] = 0.0f;
	vertex.pos[2] = 0.0f;
	vertex.color[0] = 1.0f;
	vertex.color[1] = 0.0f;
	vertex.color[2] = 0.0f;
	m_vertex.push_back(vertex);

	vertex.pos[0] = GRID_WIDTH * 2;
	vertex.pos[1] = 0.0f;
	vertex.pos[2] = 0.0f;
	vertex.color[0] = 1.0f;
	vertex.color[1] = 0.0f;
	vertex.color[2] = 0.0f;
	m_vertex.push_back(vertex);

	vertex.pos[0] = 0.0f;
	vertex.pos[1] = -GRID_WIDTH * 2;
	vertex.pos[2] = 0.0f;
	vertex.color[0] = 0.0f;
	vertex.color[1] = 1.0f;
	vertex.color[2] = 0.0f;
	m_vertex.push_back(vertex);

	vertex.pos[0] = 0.0f;
	vertex.pos[1] = GRID_WIDTH * 2;
	vertex.pos[2] = 0.0f;
	vertex.color[0] = 0.0f;
	vertex.color[1] = 1.0f;
	vertex.color[2] = 0.0f;
	m_vertex.push_back(vertex);

	vertex.pos[0] = 0.0f;
	vertex.pos[1] = 0.0f;
	vertex.pos[2] = -GRID_WIDTH * 2;
	vertex.color[0] = 0.0f;
	vertex.color[1] = 0.0f;
	vertex.color[2] = 1.0f;
	m_vertex.push_back(vertex);

	vertex.pos[0] = 0.0f;
	vertex.pos[1] = 0.0f;
	vertex.pos[2] = GRID_WIDTH * 2;
	vertex.color[0] = 0.0f;
	vertex.color[1] = 0.0f;
	vertex.color[2] = 1.0f;
	m_vertex.push_back(vertex);

	for (int i = 0; i < GRID_COUNT; i++)
	{

		//縦線----------------------------------------------------
		vertex.pos[0] = -GRID_WIDTH + (i * 20);
		vertex.pos[1] = 0.0f;
		vertex.pos[2] = -GRID_WIDTH;
		vertex.color[0] = 0.2f;
		vertex.color[1] = 0.2f;
		vertex.color[2] = 0.2f;
		m_vertex.push_back(vertex);

		vertex.pos[0] = -GRID_WIDTH + (i * 20);
		vertex.pos[1] = 0.0f;
		vertex.pos[2] = GRID_WIDTH;
		vertex.color[0] = 0.2f;
		vertex.color[1] = 0.2f;
		vertex.color[2] = 0.2f;

		m_vertex.push_back(vertex);
		//---------------------------------------------------------

		//横線----------------------------------------------------
		vertex.pos[0] = -GRID_WIDTH;
		vertex.pos[1] = 0.0f;
		vertex.pos[2] = GRID_WIDTH - (i * 20);
		vertex.color[0] = 0.2f;
		vertex.color[1] = 0.2f;
		vertex.color[2] = 0.2f;

		m_vertex.push_back(vertex);

		vertex.pos[0] = GRID_WIDTH;
		vertex.pos[1] = 0.0f;
		vertex.pos[2] = GRID_WIDTH - (i * 20);
		vertex.color[0] = 0.2f;
		vertex.color[1] = 0.2f;
		vertex.color[2] = 0.2f;

		m_vertex.push_back(vertex);
		//---------------------------------------------------------

	};

	

	D3D11_INPUT_ELEMENT_DESC vertex_desc[]{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//頂点レイアウト作成
	BackBuffer *bb = BACKBUFFER;

	//頂点レイアウト作成
	bb->GetDevice()->CreateInputLayout(
		vertex_desc,				// レイアウト設定
		ARRAYSIZE(vertex_desc),		// 配列サイズ
		bb->GetVertexShader()->GetData(),	// レイアウトと関連付ける頂点シェーダのデータ
		bb->GetVertexShader()->GetSize(),	// レイアウトと関連付ける頂点シェーダのサイズ
		&m_InputLayout);			// 作成された頂点レイアウトの格納先

	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(Vertex) * m_vertex.size();	// バッファのサイズ
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// 使用方法
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND設定
	buffer_desc.CPUAccessFlags = 0;						// リソースへのCPUのアクセス権限についての設定
	buffer_desc.MiscFlags = 0;							// リソースオプションのフラグ
	buffer_desc.StructureByteStride = 0;				// 構造体のサイズ

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = m_vertex.data();	// バッファの中身の設定
	sub_resource.SysMemPitch = 0;			// textureデータを使用する際に使用するメンバ
	sub_resource.SysMemSlicePitch = 0;		// textureデータを使用する際に使用するメンバ

	// バッファ作成
	bb->GetDevice()->CreateBuffer(
		&buffer_desc,								// バッファ情報
		&sub_resource,								// リソース情報
		&m_VertexBuffer);	// 作成されたバッファの格納先

}

void Grid::Update(void)
{
	if (CInput::GetKeyTrigger(VK_P))
		m_bDraw = !m_bDraw;
}

void Grid::Draw(void)
{
	

	BackBuffer *buffer = BACKBUFFER;
	buffer->SetUpContext(VERTEX, PIXEL, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	UINT strides = sizeof(Vertex);

	XMMATRIX mtxWorld = XMMatrixIdentity();
	// ワールドマトリクス設定
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX rotate_x = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(0.0f));
	DirectX::XMMATRIX rotate_y = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(0.0f));
	DirectX::XMMATRIX rotate_z = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(0.0f));
	DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	mtxWorld = scale_mat * rotate_x * rotate_y * rotate_z * translate;
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
		&m_VertexBuffer,		// 頂点バッファ
		&strides,				// バッファに使用している構造体のサイズ
		&offsets);				// 開始オフセッ

	m_pConstantBuffer[0]->SetVertexShader();
	m_pConstantBuffer[1]->SetPixelShader();
	// 描画
	if (!m_bDraw)
	{
		buffer->GetDeviceContext()->Draw(
			m_vertex.size(),		// 頂点数
			0);		// 開始頂点のインデックス
	}

}