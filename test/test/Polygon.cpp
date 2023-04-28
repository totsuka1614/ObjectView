/******************************************************************************
* 
* @file      Polygon.cpp
* @brief     2Dポリゴンクラス
* @author    Totsuka Kensuke
* @date      2023/04/20
* @note      HALサンプルを改良
* @attention 
* 
******************************************************************************/
#include "Polygon.h"
#include "Texture.h"

/******************************************************************************
* 
* @brief      Init
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       初期化
* @attention  
******************************************************************************/
void CPolygon::Init()
{
	//バッファ取得
	CBackBuffer* buffer = BACKBUFFER;

	HRESULT hr = S_OK;

	// シェーダ初期化
	static const D3D11_INPUT_ELEMENT_DESC vertex_desc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	
	//頂点レイアウト作成
	hr = buffer->GetDevice()->CreateInputLayout(
		vertex_desc,				// レイアウト設定
		ARRAYSIZE(vertex_desc),		// 配列サイズ
		buffer->GetVertexShader(VERTEX2D)->GetData(),	// レイアウトと関連付ける頂点シェーダのデータ
		buffer->GetVertexShader(VERTEX2D)->GetSize(),	// レイアウトと関連付ける頂点シェーダのサイズ
		&m_pInputLayout);			// 作成された頂点レイアウトの格納先

	//定数バッファ作成
	m_pConstantBuffer = new CConstantBuffer;
	m_pConstantBuffer->Create(sizeof(SHADER_GLOBAL));

	// 変換行列初期化
	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mView, XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	XMStoreFloat4x4(&m_mProj,
		XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 100.0f));
	XMStoreFloat4x4(&m_mTex, XMMatrixIdentity());
	m_mTex._44 = 0.0f;

	//初期値設定
	m_vPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vDegree = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_vColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_bInvalidate = false;
	m_fColor = 0.0f;
	m_vPosTexFrame = XMFLOAT2(0.0f, 0.0f);
	m_vSizeTexFrame = XMFLOAT2(1.0f, 1.0f);

	// 頂点情報の作成
	hr = MakeVertex(buffer->GetDevice());

}

/******************************************************************************
* 
* @brief      Fin
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       終了処理
* @attention  
******************************************************************************/
void CPolygon::Fin(void)
{
	// 頂点バッファの解放
	SAFE_RELEASE(m_pVertexBuffer);
	// 定数バッファの解放
	SAFE_DELETE(m_pConstantBuffer);
	// 頂点フォーマット解放
	SAFE_RELEASE(m_pInputLayout);
}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       描画
* @attention  
******************************************************************************/
void CPolygon::Draw()
{
	//バッファ取得
	ID3D11DeviceContext* pDeviceContext = BACKBUFFER->GetDeviceContext();

	//シェーダ,プリミティブセット
	BACKBUFFER->SetUpContext(VERTEX2D,PIXEL2D, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 拡縮
	XMMATRIX mWorld = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	// 回転
	mWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vDegree.x),
		XMConvertToRadians(m_vDegree.y), XMConvertToRadians(m_vDegree.z));
	// 移動
	mWorld *= XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	// ワールド マトリックスに設定
	XMStoreFloat4x4(&m_mWorld, mWorld);

	if (m_pTexture) {
		// 拡縮
		mWorld = XMMatrixScaling(m_vSizeTexFrame.x, m_vSizeTexFrame.y, 1.0f);
		// 移動
		mWorld *= XMMatrixTranslation(m_vPosTexFrame.x, m_vPosTexFrame.y, 0.0f);
		// テクスチャ マトリックスに設定
		XMStoreFloat4x4(&m_mTex, mWorld);
	}
	else {
		// テクスチャ無し
		m_mTex._44 = 0.0f;
	}

	// 頂点バッファ更新
	SetVertex();

	//レイアウトセット
	pDeviceContext->IASetInputLayout(m_pInputLayout);

	UINT stride = sizeof(VERTEX_2D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//テクスチャセット
	BACKBUFFER->SetTexture(m_pTexture);

	//定数バッファ設定
	SHADER_GLOBAL cb;
	cb.mProj = XMMatrixTranspose(XMLoadFloat4x4(&m_mProj));
	cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&m_mView));
	cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_mWorld));
	cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(&m_mTex));
	cb.bColor = m_fColor;
	cb.rand = rand()% 100 + 50;
	m_pConstantBuffer->Update(&cb);
	m_pConstantBuffer->SetVertexShader();
	m_pConstantBuffer->SetPixelShader();

	// ポリゴンの描画
	pDeviceContext->Draw(NUM_VERTEX, 0);
}

/******************************************************************************
* 
* @brief      MakeVertex
* @param[in]  pDevice
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       頂点作成
* @attention  
******************************************************************************/
HRESULT CPolygon::MakeVertex(ID3D11Device* pDevice)
{
	// 頂点座標の設定
	m_vertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	m_vertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	m_vertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	m_vertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);

	// 拡散反射光の設定
	m_vertexWk[0].diffuse = m_vColor;
	m_vertexWk[1].diffuse = m_vColor;
	m_vertexWk[2].diffuse = m_vColor;
	m_vertexWk[3].diffuse = m_vColor;

	// テクスチャ座標の設定
	m_vertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	m_vertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	m_vertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	m_vertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(m_vertexWk);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &m_vertexWk[0];

	HRESULT hr = pDevice->CreateBuffer(&vbd, &initData, &m_pVertexBuffer);

	return hr;
}

/******************************************************************************
* 
* @brief      SetVertex
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       頂点セット
* @attention  
******************************************************************************/
void CPolygon::SetVertex(void)
{
	if (m_bInvalidate) {
		//頂点バッファの中身を埋める
		ID3D11DeviceContext* pDeviceContext = BACKBUFFER->GetDeviceContext();
		HRESULT hr = S_OK;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		hr = pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (SUCCEEDED(hr)) {
			// 拡散反射光の設定
			m_vertexWk[0].diffuse = m_vColor;
			m_vertexWk[1].diffuse = m_vColor;
			m_vertexWk[2].diffuse = m_vColor;
			m_vertexWk[3].diffuse = m_vColor;
			// 頂点データを上書き
			memcpy_s(msr.pData, sizeof(m_vertexWk), m_vertexWk, sizeof(m_vertexWk));
			// 頂点データをアンロックする
			pDeviceContext->Unmap(m_pVertexBuffer, 0);
			// フラグをクリア
			m_bInvalidate = false;
		}
	}
}

/******************************************************************************
* 
* @brief      SetTexture
* @param[in]  pTexture
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       テクスチャセッター
* @attention  Resourceの形式
******************************************************************************/
void CPolygon::SetTexture(ID3D11ShaderResourceView* pTexture)
{
	m_pTexture = pTexture;
	m_mTex._44 = (m_pTexture) ? 1.0f : 0.0f;
}

/******************************************************************************
* 
* @brief      SetTexture
* @param[in]  path
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       テクスチャセッター
* @attention  ファイルバスの形式
******************************************************************************/
void CPolygon::SetTexture(const char* path)
{
	CreateTextureFromFile(BACKBUFFER->GetDevice(), path, &m_pTexture);
	m_mTex._44 = (m_pTexture) ? 1.0f : 0.0f;
}

/******************************************************************************
* 
* @brief      SetPos
* @param[in]  fX
* @param[in]  fY
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       ポジションセッター
* @attention  
******************************************************************************/
void CPolygon::SetPos(float fX, float fY)
{
	m_vPos.x = fX;
	m_vPos.y = fY;
}

/******************************************************************************
* 
* @brief      SetSize
* @param[in]  fScaleX
* @param[in]  fScaleY
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       スケールセッター
* @attention  
******************************************************************************/
void CPolygon::SetSize(float fScaleX, float fScaleY)
{
	m_vScale.x = fScaleX;
	m_vScale.y = fScaleY;
}

/******************************************************************************
* 
* @brief      SetAngle
* @param[in]  fAngle
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       アングルセッター
* @attention  
******************************************************************************/
void CPolygon::SetAngle(float fAngle)
{
	m_vDegree.z = fAngle;
}

/******************************************************************************
* 
* @brief      SetUV
* @param[in]  fU
* @param[in]  fV
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       UVセッター
* @attention  (0.0≦fU＜1.0, 0.0≦fV＜1.0)
******************************************************************************/
void CPolygon::SetUV(float fU, float fV)
{
	m_vPosTexFrame.x = fU;
	m_vPosTexFrame.y = fV;
}

/******************************************************************************
* 
* @brief      SetFrameSize
* @param[in]  fWidth
* @param[in]  fHeight
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       切り取る場所
* @attention  (0.0＜fWidth≦1.0, 0.0＜fHeight≦1.0)
******************************************************************************/
void CPolygon::SetFrameSize(float fWidth, float fHeight)
{
	m_vSizeTexFrame.x = fWidth;
	m_vSizeTexFrame.y = fHeight;
}

/******************************************************************************
* 
* @brief      SetColor
* @param[in]  fRed
* @param[in]  fGreen
* @param[in]  fBlue
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       カラーセッター
* @attention  RGB
******************************************************************************/
void CPolygon::SetColor(float fRed, float fGreen, float fBlue)
{
	SetColor(XMFLOAT3(fRed, fGreen, fBlue));
}

/******************************************************************************
* 
* @brief      SetColor
* @param[in]  vColor
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       カラーセッター
* @attention  RGB
******************************************************************************/
void CPolygon::SetColor(XMFLOAT3 vColor)
{
	if (vColor.x != m_vColor.x || vColor.y != m_vColor.y || vColor.z != m_vColor.z) {
		m_vColor.x = vColor.x;
		m_vColor.y = vColor.y;
		m_vColor.z = vColor.z;
		m_bInvalidate = true;
	}
}

/******************************************************************************
* 
* @brief      SetColor
* @param[in]  fRed
* @param[in]  fGreen
* @param[in]  fBlue
* @param[in]  fAlpha
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       カラーセッター
* @attention  RGBA
******************************************************************************/
void CPolygon::SetColor(float fRed, float fGreen, float fBlue, float fAlpha)
{
	SetColor(XMFLOAT4(fRed, fGreen, fBlue, fAlpha));
}

/******************************************************************************
* 
* @brief      SetColor
* @param[in]  vColor
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       カラーセッター
* @attention  RGBA
******************************************************************************/
void CPolygon::SetColor(XMFLOAT4 vColor)
{
	if (vColor.x != m_vColor.x || vColor.y != m_vColor.y || vColor.z != m_vColor.z || vColor.w != m_vColor.w) {
		m_vColor = vColor;
		m_bInvalidate = true;
	}
}

/******************************************************************************
* 
* @brief      SetAlpha
* @param[in]  fAlpha
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/20
* @note       透明度セット
* @attention  
******************************************************************************/
void CPolygon::SetAlpha(float fAlpha)
{
	if (fAlpha != m_vColor.w) {
		m_vColor.w = fAlpha;
		m_bInvalidate = true;
	}
}
