/******************************************************************************
* 
* @file      model.cpp
* @brief     モデルクラス
* @author    Totsuka Kensuke
* @date      2023/04/18
* @note      
* @attention 
* 
******************************************************************************/
#include "model.h"
#include "GUI.h"
#include <string>
#include "Input.h"

/******************************************************************************
* 
* @brief      ~Model
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       デストラクタ
* @attention  
* 
******************************************************************************/
Model::~Model()
{
	//解放
	IMGUI->GetObjectList().remove(m_Box);
}

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
void Model::Init(void)
{
	//モデルロード
	FBXFile::Load(m_cFileName);

	//初期値設定
	m_Box = new Box;
	char name[256];
	strcpy(name, m_cName);
	strcat(name, "BoxCollider");
	m_Box->SetName(name);
	m_Box->Init(XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_Box->LoadFile();

	//外部ファイルロード
	ObjectBase::LoadFile();

	//コライダー設定(モデルに追従)
	TARGET_TRANSFORM* target = new TARGET_TRANSFORM;
	m_bCol = true;
	target->pos = &m_vPos;
	target->scale = &m_vScale;
	target->deglee = &m_vDegree;
	m_Box->SetTarget(*target);
	GUI::Get()->Entry(*this);
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       終了処理
* @attention  
* 
******************************************************************************/
void Model::Uninit(void)
{
	//セーブ
	m_Box->SaveFile();
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
void Model::Update(void)
{
	// ワールドマトリクス設定
	XMMATRIX translate = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMMATRIX rotate_x = XMMatrixRotationX(XMConvertToRadians(m_vDegree.x));
	XMMATRIX rotate_y = XMMatrixRotationY(XMConvertToRadians(m_vDegree.y));
	XMMATRIX rotate_z = XMMatrixRotationZ(XMConvertToRadians(m_vDegree.z));
	XMMATRIX scale_mat =XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	m_mtxWorld = scale_mat * rotate_x * rotate_y * rotate_z * translate;	
}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       描画処理
* @attention  
* 
******************************************************************************/
void Model::Draw(void)
{
	//描画
	FBXFile::Draw();
	m_Box->ColliderDraw();
}