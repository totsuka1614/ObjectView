//=============================================================================
//
// モデル クラス [Model.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "model.h"
#include "GUI.h"
#include <string.h>
#include <string>
#include "Input.h"

#define MODEL_NAME "data/model/unitychan.fbx"

void Model::Init(void)
{
	//FBXファイルの読み込み
	m_ModelData = new FBXFile;

	m_ModelData->Load(MODEL_NAME);

	strcpy(m_cFileName, MODEL_NAME);
	
	m_mtxWorld = XMMatrixIdentity();
	m_PStype = PIXEL;
	m_vPos		= XMFLOAT3(0.0f, 10.0f, 0.0f);
	m_vDegree	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale	= XMFLOAT3(1.0f, 1.0f, 1.0f);

	bActive = false;
}

void Model::Uninit(void)
{
	delete m_ModelData;
}

void Model::Update(void)
{
	//if(CInput::GetKeyTrigger(VK_A))
	//{
	//	//--- 「ファイルを開く」ダイアログの表示
	////ダイアログの初期設定
	//	char fileName[MAX_PATH] = "";	//初期化で殻にしておかないと、最初の乱数で指定されるため、開かない
	//	OPENFILENAME ofn;
	//	ZeroMemory(&ofn, sizeof(ofn));
	//	ofn.lStructSize = sizeof(ofn);
	//	ofn.hwndOwner = GetActiveWindow();
	//	ofn.lpstrFile = fileName;			//開くで選択したファイルパスを格納
	//	ofn.nMaxFile = sizeof(fileName);
	//	//ダイアログ右下の、ファイル拡張子のフィルター設定
	//	//[表示名\0拡張子\0]の組み合わせ、\0で区切る
	//	ofn.lpstrFilter = _T("ALL Files\0*.*\0.fbx\0*fbx\0");
	//	ofn.nFilterIndex = 2;				//最初に指定しているフィルター
	//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//	//ダイアログを表示
	//	//※ファイル指定ができたらTRUEが買える
	//	if (TRUE == GetOpenFileName(&ofn))
	//	{
	//		std::string file_path = fileName;
	//		int len = strlen(fileName);
	//		int path_start_point = 1;

	//		for (int i = len - 1; i >= 0; i--)
	//		{
	//			if (fileName[i] == '\\')
	//			{
	//				path_start_point = i;
	//				break;
	//			}
	//		}
	//		file_path = file_path.substr(path_start_point + 1);

	//		}
	//}

	//m_vDegree.y += 1.0f;

	// ワールドマトリクス設定
	XMMATRIX translate = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMMATRIX rotate_x = XMMatrixRotationX(XMConvertToRadians(m_vDegree.x));
	XMMATRIX rotate_y = XMMatrixRotationY(XMConvertToRadians(m_vDegree.y));
	XMMATRIX rotate_z = XMMatrixRotationZ(XMConvertToRadians(m_vDegree.z));
	XMMATRIX scale_mat =XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	m_mtxWorld = scale_mat * rotate_x * rotate_y * rotate_z * translate;

	//m_vDegree.y += 0.5f;

	//カメラ情報
	CCamera* pCamera = CCamera::Get();

	POINT point;
	GetCursorPos(&point);
	
	//// 各行列の逆行列を算出
	//XMMATRIX InvView, InvPrj, VP, InvViewport;
	//InvView = XMMatrixInverse(NULL, XMLoadFloat4x4(&pCamera->GetViewMatrix()));
	//InvPrj = XMMatrixInverse(NULL, XMLoadFloat4x4(&pCamera->GetProjMatrix()));
	//VP = XMMatrixIdentity();
	//XMFLOAT4X4 a;
	//XMStoreFloat4x4(&a,VP);
	//a._11 = SCREEN_CENTER_X; a._22 = -SCREEN_CENTER_Y;
	//a._41 = SCREEN_CENTER_X; a._42 = SCREEN_CENTER_Y;
	//VP = XMLoadFloat4x4(&a);
	//InvViewport = XMMatrixInverse(NULL,VP);

	//// 逆変換
	//XMMATRIX tmp = InvViewport * InvPrj * InvView;
	//XMVECTOR b = XMLoadFloat3(&XMFLOAT3(point.x, point.y, 1.0f));
	//XMVECTOR out = XMVector3TransformCoord(b, tmp);
	//XMFLOAT3 c; XMStoreFloat3(&c, out);



	GUI::Get()->Display(*this);
}

void Model::Draw(void)
{
	if (m_ModelData)
	{
		m_ModelData->Draw(m_mtxWorld, m_PStype);
	}
}