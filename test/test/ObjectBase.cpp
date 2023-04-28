/******************************************************************************
* 
* @file      ObjectBase.cpp
* @brief     オブジェクトベースクラス
* @author    Totsuka Kensuke
* @date      2023/04/18
* @note      
* @attention 
* 
******************************************************************************/
#include "ObjectBase.h"
#include "ConstantBuffer.h"
#include "BackBuffer.h"
#include "CollisionList.h"
#include "GlobalData.h"
#include "Camera.h"

/******************************************************************************
* 
* @brief      Init
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       初期化
* @attention  
* 
******************************************************************************/
void CObjectBase::Init()
{
	//コリジョンリスト登録
	CColList::Get()->SetObj(*this);

	//テクスチャがあれば生成
	if (m_bTexture)
		if (CreateTextureFromFile(BACKBUFFER->GetDevice(), m_cTexturePath, &m_pTexrure) != S_OK)
			m_bTexture = false;
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       更新処理
* @attention  
* 
******************************************************************************/
void CObjectBase::Update()
{
	//バッファ取得
	CBackBuffer* buffer = BACKBUFFER;

	//定数バッファ生成
	CConstantBuffer* cb = new CConstantBuffer;
	cb->Create(sizeof(XMFLOAT4));

	//テクスチャ設定
	if (m_bTexture)
		buffer->SetTexture(m_pTexrure);
	else
		buffer->SetTexture(NULL);
	
	//影の設定
	switch (m_VStype)
	{
	case SHADOWVS:
		//バッファ設定
		D3D11_BUFFER_DESC buffer_desc;
		ID3D11Buffer* cb;
		buffer_desc.ByteWidth = sizeof(XMFLOAT4X4) * 2;
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		HRESULT hr;
		//バッファ作成
		hr = BACKBUFFER->GetDevice()->CreateBuffer(&buffer_desc, nullptr, &cb);

		//光源設定
		XMFLOAT4X4 mat[2];
		CCamera::Get()->SetSun();
		DirectX::XMMATRIX camMat[2];
		camMat[0] = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetViewMatrix());
		camMat[1] = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetProjMatrix());
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(camMat[0]));
		DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixTranspose(camMat[1]));
		BACKBUFFER->GetDeviceContext()->UpdateSubresource(cb, 0, nullptr, mat, 0, 0);
		BACKBUFFER->GetDeviceContext()->VSSetConstantBuffers(1, 1, &cb);
		CCamera::Set();
		break;
	default:
		break;
	}

	//マップ画像設定
	switch (m_PStype)
	{
	case DISSOLVE:
		//ディソルブ設定
		buffer->SetTexture(buffer->GetTexture(DISSOLVE_MAP),1);
		cb->Update(&m_fRate);
		cb->SetPixelShader(1);
		break;
	case BUMPMAP:
		//伴ぷマップ設定
		buffer->SetTexture(buffer->GetTexture(BUMP_MAP),1);
		break;
	case TOONPS:
		//トゥーンせってい
		buffer->SetTexture(buffer->GetTexture(LAMP_MAP), 1);
		break;
	case DEPTHSHADOWPS:
		break;
	default:
		break;

	}

	delete cb;
}

/******************************************************************************
* 
* @brief      SaveFile
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       セーブ関数
* @attention  
* 
******************************************************************************/
void CObjectBase::SaveFile()
{
	//ファイルポインタ
	FILE* fp;

	//data/save/ファイル名.totsukaの形式に
	char path[256] = "data/save/";
	strcat(path, m_cName);
	strcat(path, ".totsuka");

	//ファイルオープン
	fopen_s(&fp, path, "wb");
	if (fp)
	{
		//セーブ
		fwrite(this, sizeof(CObjectBase), 1, fp);
		fclose(fp);
	}
}

/******************************************************************************
* 
* @brief      LoadFile
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       ロード関数
* @attention  
* 
******************************************************************************/
void CObjectBase::LoadFile()
{
	//オブジェクト生成
	CObjectBase ob;

	//ファイルポインタ
	FILE* fp;

	//data/save/ファイル名.totsukaの形式に
	char path[256] = "data/save/";
	strcat(path, m_cName);
	strcat(path, ".totsuka");

	//ファイルオープン
	fopen_s(&fp, path, "rb");
	if (fp)
	{
		//ロード
		fread(&ob, sizeof(CObjectBase), 1, fp);
		fclose(fp);
		*this = ob;
	}

}

/******************************************************************************
* 
* @brief      LoadFile
* @param[in]  save
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       ロード
* @attention  引数で設定したオブジェクトをセット
* 
******************************************************************************/
void CObjectBase::LoadFile(CObjectBase save)
{
	*this = save;
}

/******************************************************************************
* 
* @brief      CollisionTo3D
* @param[in]  obj
* @return     bool
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       あたり判定
* @attention  
* 
******************************************************************************/
bool CObjectBase::CollisionTo3D(CObjectBase* obj)
{
	if (obj == this) return false;
	if (!m_bEnable || !obj)
		return false;

	// 中心座標を求める
	XMFLOAT3 vA, vB;
	XMStoreFloat3(&vA,
		XMVector3TransformCoord(
			XMLoadFloat3(&m_vPos),
			m_mtxWorld));
	XMStoreFloat3(&vB,
		XMVector3TransformCoord(
			XMLoadFloat3(&obj->m_vPos),
			obj->m_mtxWorld));
	// 衝突判定
	XMFLOAT3& vAr = m_vScale;
	XMFLOAT3& vBr = obj->GetScale();
	return vA.x - vAr.x <= vB.x + vBr.x &&
		vB.x - vBr.x <= vA.x + vAr.x &&
		vA.y - vAr.y <= vB.y + vBr.y &&
		vB.y - vBr.y <= vA.y + vAr.y &&
		vA.z - vAr.z <= vB.z + vBr.z &&
		vB.z - vBr.z <= vA.z + vAr.z;
}