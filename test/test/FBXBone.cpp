#include "FBX.h"

static FbxManager *g_pManager = NULL;


//-----------------------------------------------------
//
/// @brief データクリア
//
//-----------------------------------------------------
void FBXFile::CleanupBone(BoneListInfo &boneListInfo)
{
	BoneList::iterator it = boneListInfo.list.begin();
	while (it != boneListInfo.list.end())
	{
		it->name.clear();
		it->translate.x = it->translate.y = it->translate.z = 0.0f;
		it->rotation.x = it->rotation.y = it->translate.z = 0.0f;
		it->scale.x = it->scale.y = it->scale.z = 0.0f;
		it->preRotate.x = it->preRotate.y = it->preRotate.z = 0.0f;
		it->order = ROTATE_XYZ;
		it->children.clear();
		++it;
	}
	boneListInfo.list.clear();
}

//-----------------------------------------------------
//
/// @brief データ取得
//
//-----------------------------------------------------
void FBXFile::GetBone(FbxScene *pScene, BoneListInfo &boneListInfo, bool isMirror)
{
	CleanupBone(boneListInfo);
	boneListInfo.list.resize(pScene->GetMemberCount<FbxSkeleton>());
	BoneList::iterator it = boneListInfo.list.begin();
	while (it != boneListInfo.list.end())
	{
		FbxNode *pNode = pScene->GetMember<FbxSkeleton>(static_cast<int>(it - boneListInfo.list.begin()))->GetNode();
		FbxBoneInfo &info = *it;

		// ボーン情報取得
		info.name = pNode->GetInitialName();
		FbxDouble3 trans = pNode->LclTranslation.Get();
		FbxDouble3 rotate = pNode->LclRotation.Get();
		FbxDouble3 scale = pNode->LclScaling.Get();
		FbxDouble3 preRotate = pNode->PreRotation.Get();
		info.translate.x = static_cast<float>(trans[0]) * (isMirror ? -1.0f : 1.0f);
		info.translate.y = static_cast<float>(trans[1]);
		info.translate.z = static_cast<float>(trans[2]);
		info.rotation.x = static_cast<float>(rotate[0]);
		info.rotation.y = static_cast<float>(rotate[1]) * (isMirror ? -1.0f : 1.0f);
		info.rotation.z = static_cast<float>(rotate[2]) * (isMirror ? -1.0f : 1.0f);
		info.scale.x = static_cast<float>(scale[0]);
		info.scale.y = static_cast<float>(scale[1]);
		info.scale.z = static_cast<float>(scale[2]);
		info.preRotate.x = static_cast<float>(preRotate[0]);
		info.preRotate.y = static_cast<float>(preRotate[1]) * (isMirror ? -1.0f : 1.0f);
		info.preRotate.z = static_cast<float>(preRotate[2]) * (isMirror ? -1.0f : 1.0f);

		FbxEuler::EOrder order;
		pNode->GetRotationOrder(FbxNode::eSourcePivot, order);
		switch (order)
		{
		case FbxEuler::EOrder::eOrderXYZ: info.order = ROTATE_XYZ; break;
		case FbxEuler::EOrder::eOrderZXY: info.order = ROTATE_ZXY; break;
		default: assert(""); break;
		}

		++it;
	}

	// 子要素の構築
	it = boneListInfo.list.begin();
	while (it != boneListInfo.list.end())
	{
		FbxNode *pNode = pScene->GetMember<FbxSkeleton>(static_cast<int>(it - boneListInfo.list.begin()))->GetNode();
		FbxBoneInfo &info = *it;

		// 構築
		for (int i = 0; i < pNode->GetChildCount(); ++i)
		{
			BoneList::iterator find = boneListInfo.list.begin();
			while (find != boneListInfo.list.end())
			{
				if (find->name == pNode->GetChild(i)->GetInitialName())
				{
					info.children.push_back(static_cast<int>(find - boneListInfo.list.begin()));
					break;
				}
				++find;
			}
		}
		++it;
	}

	// ルートボーン
	boneListInfo.root = GetRootBone(pScene->GetRootNode(), boneListInfo.list);
}


int FBXFile::GetRootBone(FbxNode *pNode, BoneList &list)
{
	int attrCnt = pNode->GetNodeAttributeCount();
	for (int i = 0; i < attrCnt; ++i)
	{
		FbxNodeAttribute* attr = pNode->GetNodeAttributeByIndex(i);
		if (attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			FbxNode *pBone = attr->GetNode();
			if (pBone != NULL)
			{
				const char *nodeName = pBone->GetInitialName();
				BoneList::iterator it = list.begin();
				while (it != list.end())
				{
					if (nodeName == it->name)
					{
						FbxNode *pP = pBone->GetParent();
						nodeName = pP->GetInitialName();
						return static_cast<int>(it - list.begin());
					}
					++it;
				}
			}
		}
	}

	int root = -1;
	for (int i = 0; i < pNode->GetChildCount() && root < 0; ++i)
	{
		root = GetRootBone(pNode->GetChild(i), list);
	}
	return root;
}

void FBXFile::LoadBone(void)
{
	// 読み込み
	m_pFBXBone = new BoneListInfo;

	// 読み込み初期化
	g_pManager = FbxManager::Create();

	FbxIOSettings *pIO = FbxIOSettings::Create(g_pManager, IOSROOT);
	g_pManager->SetIOSettings(pIO);
	FbxImporter *pImporter = FbxImporter::Create(g_pManager, "");
	if (!pImporter->Initialize(m_cFileName, -1, g_pManager->GetIOSettings()))
	{
		return;
	}

	// 読み込み
	FbxScene *pScene = FbxScene::Create(g_pManager, "originalScene");
	pImporter->Import(pScene);
	// 三角化
	FbxGeometryConverter geometryConverter(g_pManager);
	geometryConverter.Triangulate(pScene, true);
	geometryConverter.RemoveBadPolygonsFromMeshes(pScene);
	// マテリアルで分割
	geometryConverter.SplitMeshesPerMaterial(pScene, true);

	GetBone(pScene, *m_pFBXBone, false);



	pImporter->Destroy();
}

/// @param index 骨の番号
/// @param parent 親の姿勢情報
void FBXFile::CalcBone(int index, DirectX::XMMATRIX parent)
{
	//--------------
	// 骨の姿勢計算
	//--------------
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(
		m_pBones[index].param.translate.x,
		m_pBones[index].param.translate.y,
		m_pBones[index].param.translate.z
	);

	DirectX::XMMATRIX R =
		DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_pBones[index].param.rotation.x)) *
		DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_pBones[index].param.rotation.y)) *
		DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_pBones[index].param.rotation.z));

	DirectX::XMMATRIX S =
		DirectX::XMMatrixScaling(
			m_pBones[index].param.scale.x,
			m_pBones[index].param.scale.y,
			m_pBones[index].param.scale.z
		);
	// 骨の姿勢 = 計算したい骨のローカル姿勢 * 今まで姿勢を計算してきた全ての親の姿勢
	// ※腕の骨なら = 腕の骨のローカル * 全ての骨(二の腕 * 上半身 * 腰)の姿勢
	DirectX::XMMATRIX mat = S * R * m_pBones[index].preRotate * T * parent;

	// 計算済みの骨情報を保存
	DirectX::XMStoreFloat4x4(&m_pBones[index].matrix, mat);

	// 次に続く骨の計算
	// ※左手首を計算したあとは、[親指、人差し指、中指、薬指、小指]と計算が続く
	for (int i = 0; i < m_pFBXBone->list[index].children.size(); ++i)
	{
		CalcBone(m_pFBXBone->list[index].children[i], mat);
	}

	//--------------
}