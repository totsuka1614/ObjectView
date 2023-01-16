#include "FBX.h"
#include <algorithm>

static const int TriangleIndex[2][3] = { { 0, 1, 2 }, {0, 2, 1} };
static FbxManager *g_pManager = NULL;
//-----------------------------------------------------
//
/// @brief データクリア
//
//-----------------------------------------------------
void FBXFile::CleanupMesh(MeshList &meshList)
{
	MeshList::iterator it = meshList.begin();
	while (it != meshList.end())
	{
		it->indexList.clear();
		it->vertexList.clear();
		it->normalInfo.list.clear();
		MeshInfo::UVList::iterator itUV = it->uvList.begin();
		while (itUV != it->uvList.end())
		{
			itUV->list.clear();
			++itUV;
		}
		it->materialName.clear();
		it->skinInfo.weightList.clear();
		it->skinInfo.boneList.clear();
		++it;
	}
	meshList.clear();
}

//-----------------------------------------------------
//
/// @brief データ取得
//
//-----------------------------------------------------
void FBXFile::GetMesh(FbxScene *pScene, MeshList &meshList, bool isMirror)
{
	CleanupMesh(meshList);
	meshList.resize(pScene->GetMemberCount<FbxMesh>());
	MeshList::iterator it = meshList.begin();

	while (it != meshList.end())
	{
		FbxMesh *pMesh = pScene->GetMember<FbxMesh>(static_cast<int>(it - meshList.begin()));
		MeshInfo &info = *it;

		GetIndex(info.indexList, pMesh, isMirror);
		GetVertex(info.vertexList, pMesh, isMirror);
		GetNormal(info.normalInfo, pMesh, info.indexList, info.vertexList, isMirror);
		GetUV(info.uvList, pMesh, info.indexList, info.vertexList, isMirror);
		GetMaterial(info.materialName, pMesh, pScene->GetRootNode());
		//GetSkin(info.skinInfo, pMesh, info.vertexList, isMirror);
		GetTransform(info.transform, pMesh, isMirror);

		++it;
	}
}

//-----------------------------------------------------
//
/// @brief インデックスを取得
//
//-----------------------------------------------------
void FBXFile::GetIndex(MeshInfo::IndexList &list, FbxMesh *pMesh, bool isMirror)
{
	list.resize(pMesh->GetPolygonCount() * 3);

	for (int i = 0; i < pMesh->GetPolygonCount(); ++i)
	{
	
		for (int j = 0; j < 3; ++j)
		{
			list[i * 3 + j] = pMesh->GetPolygonVertex(i, TriangleIndex[isMirror][j]);
		}
	}
}


//-----------------------------------------------------
//
// 頂点情報を取得
//
//-----------------------------------------------------
void FBXFile::GetVertex(MeshInfo::VertexList &list, FbxMesh *pMesh, bool isMirror)
{
	list.resize(pMesh->GetControlPointsCount());
	FbxVector4 *pVertices = pMesh->GetControlPoints();
	MeshInfo::VertexList::iterator it = list.begin();
	while (it != list.end())
	{
		int index = static_cast<int>(it - list.begin());
		it->x = static_cast<float>(pVertices[index][0]) * (isMirror ? -1.0f : 1.0f);
		it->y = static_cast<float>(pVertices[index][1]);
		it->z = static_cast<float>(pVertices[index][2]);
		++it;
	}
}

//-----------------------------------------------------
//
/// @brief 法線情報を取得
//
//-----------------------------------------------------
void FBXFile::GetNormal(MeshInfo::NormalInfo &info, FbxMesh *pMesh, MeshInfo::IndexList &idxList, MeshInfo::VertexList &vtxList, bool isMirror)
{
	FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal();
	if (pNormal == NULL) {
		return;
	}

	FbxLayerElement::EMappingMode mapping = pNormal->GetMappingMode();
	FbxLayerElement::EReferenceMode reference = pNormal->GetReferenceMode();

	// マッピング別に初期化
	if (mapping == FbxGeometryElement::eByControlPoint)
	{
		info.mapping = MAPPING_VERTEX;
		info.list.resize(vtxList.size());
	}
	else if (mapping == FbxGeometryElement::eByPolygonVertex)
	{
		info.mapping = MAPPING_INDEX;
		info.list.resize(idxList.size());
	}

	MeshInfo::NormalInfo::List::iterator it = info.list.begin();
	while (it != info.list.end())
	{
		int index = static_cast<int>(it - info.list.begin());
		if (mapping == FbxGeometryElement::eByPolygonVertex)
		{
			index = index / 3 * 3 + TriangleIndex[isMirror][index % 3];
		}
		if (reference == FbxGeometryElement::eIndexToDirect)
		{
			index = pNormal->GetIndexArray().GetAt(index); // インデックス対応
		}
		FbxVector4 normal = pNormal->GetDirectArray().GetAt(index);
		it->x = static_cast<float>(normal[0]) * (isMirror ? -1.0f : 1.0f);
		it->y = static_cast<float>(normal[1]);
		it->z = static_cast<float>(normal[2]);
		++it;
	}
}

//-----------------------------------------------------
//
/// @brief UV情報を取得
//
//-----------------------------------------------------
void FBXFile::GetUV(MeshInfo::UVList &list, FbxMesh *pMesh, MeshInfo::IndexList &idxList, MeshInfo::VertexList &vtxList, bool isMirror)
{
	int uvCount = pMesh->GetElementUVCount();
	if (uvCount <= 0) {
		return;
	}

	list.resize(uvCount);
	MeshInfo::UVList::iterator it = list.begin();
	while (it != list.end())
	{
		int index = static_cast<int>(it - list.begin());
		FbxGeometryElementUV* pUV = pMesh->GetElementUV(index);
		if (pUV)
		{
			FbxLayerElement::EMappingMode mapping = pUV->GetMappingMode();
			FbxLayerElement::EReferenceMode reference = pUV->GetReferenceMode();

			// マッピング別に初期化
			if (mapping == FbxGeometryElement::eByControlPoint)
			{
				it->mapping = MAPPING_VERTEX;
				it->list.resize(vtxList.size());
			}
			else if (mapping == FbxGeometryElement::eByPolygonVertex)
			{
				it->mapping = MAPPING_INDEX;
				it->list.resize(idxList.size());
			}
			MeshInfo::UVInfo::List::iterator itUV = it->list.begin();
			while (itUV != it->list.end())
			{
				int uvIndex = static_cast<int>(itUV - it->list.begin());
				if (reference == FbxGeometryElement::eIndexToDirect)
				{
					uvIndex = uvIndex / 3 * 3 + TriangleIndex[isMirror][uvIndex % 3];
					uvIndex = pUV->GetIndexArray().GetAt(uvIndex); // インデックス対応
				}
				FbxVector2 uv = pUV->GetDirectArray().GetAt(uvIndex);
				itUV->x = static_cast<float>(uv[0]);
				itUV->y = static_cast<float>(uv[1]);

				++itUV;
			}
		}
		++it;
	}
}

//-----------------------------------------------------
//
/// @brief マテリアル名を取得
//
//-----------------------------------------------------
void FBXFile::GetMaterial(MeshInfo::MaterialName &name, FbxMesh *pMesh, FbxNode *pNode)
{
	int attrCnt = pNode->GetNodeAttributeCount();
	for (int i = 0; i < attrCnt; ++i)
	{
		FbxNodeAttribute* attr = pNode->GetNodeAttributeByIndex(i);
		if (attr == pMesh)
		{
			if (attr->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				if (pNode->GetMaterialCount() > 0)
				{
					name = pNode->GetMaterial(i)->GetInitialName();
				}
				return;
			}
		}
	}

	// 子要素の探索
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		GetMaterial(name, pMesh, pNode->GetChild(i));
	}
}

//-----------------------------------------------------
//
/// @brief 頂点ブレンドを取得
//
//-----------------------------------------------------
void FBXFile::GetSkin(SkinInfo &info, FbxMesh *pMesh, std::vector<VERTEX_3D> &vtxList, bool isMirror)
{
	int skinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount > 0)
	{

		// メッシュに紐づいたボーンの情報を取り出す
		typedef std::vector<SkinInfo::WeightInfo::WeightValue> WeightList;
		typedef std::vector<WeightList> VtxWeightList;
		VtxWeightList vtxWeight(vtxList.size());
		FbxSkin* pSkin = static_cast<FbxSkin*>(pMesh->GetDeformer(0, FbxDeformer::eSkin)); // 単一のスキンのみ
		info.boneList.resize(pSkin->GetClusterCount());
		SkinInfo::BoneList::iterator itBone = info.boneList.begin();
		while (itBone != info.boneList.end())
		{
			int index = static_cast<int>(itBone - info.boneList.begin());
			// ボーン情報を取得
			FbxCluster* pCluster = pSkin->GetCluster(index);
			FbxAMatrix mat;
			pCluster->GetTransformLinkMatrix(mat);
			itBone->name = pCluster->GetLink()->GetInitialName();
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					itBone->offset[i][j] = static_cast<float>(mat[i][j]);
				}
			}
			if (isMirror)
			{
				itBone->offset[1][0] *= -1.0f;
				itBone->offset[2][0] *= -1.0f;
				itBone->offset[3][0] *= -1.0f;
				itBone->offset[0][1] *= -1.0f;
				itBone->offset[0][2] *= -1.0f;
				itBone->offset[0][3] *= -1.0f;
			}

			// 対応する頂点ブレンドを取得(データがあるだけ
			int* pIndices = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();
			for (int i = 0; i < pCluster->GetControlPointIndicesCount(); ++i)
			{
				SkinInfo::WeightInfo::WeightValue weightInfo = { index, static_cast<float>(pWeights[i]) };
				vtxWeight[pIndices[i]].push_back(weightInfo);
			}
			++itBone;
		}

		// 取り出したデータをMeshInfo::WeightSet::WEIGHT_NUM_MAX分にそろえる
		info.weightList.resize(vtxList.size());
		VtxWeightList::iterator itWeight = vtxWeight.begin();
		while (itWeight != vtxWeight.end())
		{
			// ソート
			std::sort(
				itWeight->begin(), itWeight->end(),
				[](const SkinInfo::WeightInfo::WeightValue& objA, const SkinInfo::WeightInfo::WeightValue& objB)
			{
				return objA.weight > objB.weight;
			});
			// 頂点ブレンドデータの数をそろえる
			while (itWeight->size() > SkinInfo::WeightInfo::WEIGHT_NUM_MAX)
			{
				itWeight->pop_back();
			}
			while (itWeight->size() < SkinInfo::WeightInfo::WEIGHT_NUM_MAX)
			{
				SkinInfo::WeightInfo::WeightValue weightValue = { 0, 0.0f };
				itWeight->push_back(weightValue);
			}
			// 正規化
			int index = static_cast<int>(itWeight - vtxWeight.begin());
			float total = 0.0f;
			for (int i = 0; i < SkinInfo::WeightInfo::WEIGHT_NUM_MAX; ++i)
			{
				info.weightList[index].value[i].index = (*itWeight)[i].index;
				total += (*itWeight)[i].weight;
			}
			if (total == 0.0f)
			{
				total = 1.0f;
			}
			for (int i = 0; i < SkinInfo::WeightInfo::WEIGHT_NUM_MAX; ++i)
			{
				info.weightList[index].value[i].weight = (*itWeight)[i].weight / total;
			}

			++itWeight;
		}
	}
	else
	{
		// ほぼほぼユニティちゃんのための対応かも
		// ボーンのノードに紐づいているかチェック
		FbxNode* pMeshNode = pMesh->GetNode();
		FbxNode* parent = pMeshNode->GetParent();
		while (parent != NULL)
		{
			FbxNodeAttribute* attr = parent->GetNodeAttribute();
			if (attr != NULL)
			{
				FbxNodeAttribute::EType type = attr->GetAttributeType();
				if (type == FbxNodeAttribute::eSkeleton)
				{
					break;
				}
				else if (type == FbxNodeAttribute::eMesh)
				{
					pMeshNode = parent;
				}
			}
			parent = parent->GetParent();
		}
		// 紐づいていた場合、そのボーンに完全依存する頂点ブレンドとして扱う
		if (parent != NULL)
		{
			// ボーン情報設定
			SkinInfo::BoneInfo boneInfo;
			FbxAMatrix mat = pMeshNode->EvaluateLocalTransform();
			mat = mat.Inverse();
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					boneInfo.offset[i][j] = static_cast<float>(mat[i][j]);
				}
			}
			if (isMirror)
			{
				boneInfo.offset[1][0] *= -1.0f;
				boneInfo.offset[2][0] *= -1.0f;
				boneInfo.offset[3][0] *= -1.0f;
				boneInfo.offset[0][1] *= -1.0f;
				boneInfo.offset[0][2] *= -1.0f;
				boneInfo.offset[0][3] *= -1.0f;
			}
			boneInfo.name = parent->GetInitialName();
			info.boneList.push_back(boneInfo);

			// 頂点ブレンド設定
			info.weightList.resize(vtxList.size());
			SkinInfo::WeightList::iterator it = info.weightList.begin();
			while (it != info.weightList.end())
			{
				for (int i = 0; i < SkinInfo::WeightInfo::WEIGHT_NUM_MAX; ++i)
				{
					it->value[i].index = 0;
					it->value[i].weight = 0.0f;
				}
				it->value[0].weight = 1.0f;
				++it;
			}
		}
	}
}

//-----------------------------------------------------
//
/// @brief 姿勢行列の取得
//
//-----------------------------------------------------
void FBXFile::GetTransform(Matrix& mat, FbxMesh* pMesh, bool isMirror)
{
	FbxNode* pNode = pMesh->GetNode();
	FbxNode* parent = pNode->GetParent();
	while (parent != NULL)
	{
		FbxNodeAttribute* attr = parent->GetNodeAttribute();
		if (attr != NULL)
		{
			FbxNodeAttribute::EType type = attr->GetAttributeType();
			if (type == FbxNodeAttribute::EType::eSkeleton)
			{
				// スケルトンの子階層だと、トランスフォームを無視してもよい？
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < 4; ++j) {
						mat.m[i][j] = static_cast<float>(i == j ? 1.0f : 0.0f);
					}
				}
				if (isMirror)
				{
					mat._12 *= -1.0f;
					mat._13 *= -1.0f;
					mat._14 *= -1.0f;
					mat._21 *= -1.0f;
					mat._31 *= -1.0f;
					mat._41 *= -1.0f;
				}
				return;
			}
		}
		parent = parent->GetParent();
	}

	FbxAMatrix transform(
		FbxVector4(pNode->LclTranslation),
		FbxVector4(pNode->LclRotation),
		FbxVector4(pNode->LclScaling));
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			mat.m[i][j] = static_cast<float>(transform[i][j]);
		}
	}
	if (isMirror)
	{
		mat._12 *= -1.0f;
		mat._13 *= -1.0f;
		mat._14 *= -1.0f;
		mat._21 *= -1.0f;
		mat._31 *= -1.0f;
		mat._41 *= -1.0f;
	}
}

void FBXFile::LoadMesh(void)
{
	MeshList mesh;

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

	GetMesh(pScene, mesh, false);

	pImporter->Destroy();

	// バッファ作成
	m_meshNum = mesh.size();
	auto meshIt = mesh.begin();
	while (meshIt != mesh.end())
	{
		// インデックスバッファを利用して描画するか検証
		bool isIdxDraw;
		isIdxDraw = true;
		if (meshIt->normalInfo.mapping == MAPPING_INDEX)
		{
			isIdxDraw = false;
		}
		if (meshIt->uvList.size() > 0 && meshIt->uvList[0].mapping == MAPPING_INDEX)
		{
			// 単一uvのみに対応
			isIdxDraw = false;
		}

		// 頂点バッファ作成
		int vtxNum;
		if (isIdxDraw)
		{
			vtxNum = static_cast<int>(meshIt->vertexList.size());
		}
		else
		{
			vtxNum = static_cast<int>(meshIt->indexList.size());
		}
		FBXVertex* pVtx = new FBXVertex[vtxNum];
		FBXVertex* pVtxTop = pVtx;
		for (int i = 0; i < vtxNum; ++i)
		{
			// それぞれのインデックスを取得
			int vtxIdx = i;
			int normIdx = i;
			int uvIdx = i;
			if (!isIdxDraw)
			{
				vtxIdx = meshIt->indexList[i];
				if (meshIt->normalInfo.mapping == MAPPING_VERTEX)
				{
					normIdx = vtxIdx;
				}
				if (meshIt->uvList.size() > 0 && meshIt->uvList[0].mapping == MAPPING_VERTEX)
				{
					uvIdx = vtxIdx;
				}
			}

			// 頂点
			DirectX::XMVECTOR vec;
			vec = DirectX::XMVector3TransformCoord(
				DirectX::XMVectorSet(meshIt->vertexList[vtxIdx].x, meshIt->vertexList[vtxIdx].y, meshIt->vertexList[vtxIdx].z, 0),
				DirectX::XMMatrixSet(
					meshIt->transform._11, meshIt->transform._12, meshIt->transform._13, meshIt->transform._14,
					meshIt->transform._21, meshIt->transform._22, meshIt->transform._23, meshIt->transform._24,
					meshIt->transform._31, meshIt->transform._32, meshIt->transform._33, meshIt->transform._34,
					meshIt->transform._41, meshIt->transform._42, meshIt->transform._43, meshIt->transform._44)
			);
			DirectX::XMStoreFloat3(&pVtx->pos, vec);
			// カラー
			pVtx->color = DirectX::XMFLOAT4(1, 1, 1, 1);
			// UV
			if (meshIt->uvList.size() > 0)
			{
				pVtx->uv.x = meshIt->uvList[0].list[uvIdx].x;
				pVtx->uv.y = 1.0f - meshIt->uvList[0].list[uvIdx].y;
			}
			else
			{
				pVtx->uv = DirectX::XMFLOAT2(0.0f, 0.0f);
			}
			// 法線
			if (meshIt->normalInfo.list.size() > 0)
			{
				vec = DirectX::XMVector3TransformCoord(
					DirectX::XMVectorSet(meshIt->normalInfo.list[normIdx].x, meshIt->normalInfo.list[normIdx].y, meshIt->normalInfo.list[normIdx].z, 0),
					DirectX::XMMatrixSet(
						meshIt->transform._11, meshIt->transform._12, meshIt->transform._13, 0,
						meshIt->transform._21, meshIt->transform._22, meshIt->transform._23, 0,
						meshIt->transform._31, meshIt->transform._32, meshIt->transform._33, 0,
						0, 0, 0, 1)
				);
				DirectX::XMStoreFloat3(&pVtx->normal, vec);
			}
			else
			{
				pVtx->normal = DirectX::XMFLOAT3(0, 0, 1);
			}
			// ウェイト
			if (meshIt->skinInfo.weightList.size() > 0)
			{
				for (int j = 0; j < MeshInfo::SkinInfo::WeightInfo::WEIGHT_NUM_MAX; ++j)
				{
					auto value = meshIt->skinInfo.weightList[vtxIdx].value[j];
					pVtx->weight[j] = value.weight;
					pVtx->index[j] = value.index;
				}
			}
			else
			{
				for (int j = 0; j < MeshInfo::SkinInfo::WeightInfo::WEIGHT_NUM_MAX; ++j)
				{
					pVtx->weight[j] = 0.0f;
					pVtx->index[j] = 0;
				}
				pVtx->weight[0] = 1.0f;
			}

			++pVtx;
		}

		if (isIdxDraw)
		{
			long* pIdx = new long[meshIt->indexList.size()];
			for (int i = 0; i < meshIt->indexList.size(); ++i)
			{
				pIdx[i] = meshIt->indexList[i];
			}
			delete[] pIdx;
		}
		else
		{
		}
		delete[] pVtxTop;

		++meshIt;
	}





	// ゲームで使用する形に吸出し
	/*
	 STL ... 標準テンプレートライブラリ(Standard Template Library)
	 std::vector 動的配列
	 std::list 動的リスト
	*/
	int boneNum = m_pFBXBone->list.size();
	m_pBones = new BoneInfo[boneNum];
	m_pBlendBones[0] = new MatrixParam[boneNum];
	m_pBlendBones[1] = new MatrixParam[boneNum];
	for (int i = 0; i < m_pFBXBone->list.size(); ++i)
	{
		DirectX::XMStoreFloat4x4(
			&m_pBones[i].matrix, DirectX::XMMatrixIdentity());
		// 事前回転の計算
		DirectX::XMMATRIX rotX =
			DirectX::XMMatrixRotationX(
				DirectX::XMConvertToRadians(m_pFBXBone->list[i].preRotate.x));
		DirectX::XMMATRIX rotY =
			DirectX::XMMatrixRotationY(
				DirectX::XMConvertToRadians(m_pFBXBone->list[i].preRotate.y));
		DirectX::XMMATRIX rotZ =
			DirectX::XMMatrixRotationZ(
				DirectX::XMConvertToRadians(m_pFBXBone->list[i].preRotate.z));
		m_pBones[i].preRotate = rotX * rotY * rotZ;
		// FBXから読み込んだ骨の初期姿勢を、ゲーム内で使用する骨にコピーする
		m_pBones[i].param.translate.x = m_pFBXBone->list[i].translate.x;
		m_pBones[i].param.translate.y = m_pFBXBone->list[i].translate.y;
		m_pBones[i].param.translate.z = m_pFBXBone->list[i].translate.z;
		m_pBones[i].param.rotation.x = m_pFBXBone->list[i].rotation.x;
		m_pBones[i].param.rotation.y = m_pFBXBone->list[i].rotation.y;
		m_pBones[i].param.rotation.z = m_pFBXBone->list[i].rotation.z;
		m_pBones[i].param.scale.x = m_pFBXBone->list[i].scale.x;
		m_pBones[i].param.scale.y = m_pFBXBone->list[i].scale.y;
		m_pBones[i].param.scale.z = m_pFBXBone->list[i].scale.z;
		// 安全処理(何も中身が初期化されてない状態を避ける)で
		// 遷移用の骨に初期姿勢の情報を設定
		m_pBlendBones[0][i] = m_pBones[i].param;
		m_pBlendBones[1][i] = m_pBones[i].param;
	}

	// スキンメッシュ用のデータ構築
	// (メッシュとボーンを結びつける
	meshIt = mesh.begin();
	while (meshIt != mesh.end())
	{
		if (meshIt->skinInfo.boneList.size() > 0)
		{
			MeshInverse meshInv;
			meshInv.num = meshIt->skinInfo.boneList.size();
			meshInv.pList = new MeshInverse::List[meshInv.num];
			for (int i = 0; i < meshInv.num; ++i)
			{
				// メッシュに紐づいてる骨が、骨全体の配列の
				// どの位置に所属しているか
				for (int j = 0; j < m_pFBXBone->list.size(); ++j)
				{
					if (meshIt->skinInfo.boneList[i].name ==
						m_pFBXBone->list[j].name)
					{
						meshInv.pList[i].boneIndex = j;
						// 逆行列の計算
						DirectX::XMFLOAT4X4 offset(
							meshIt->skinInfo.boneList[i].offset[0]);
						meshInv.pList[i].invMat =
							DirectX::XMMatrixInverse(nullptr,
								DirectX::XMLoadFloat4x4(&offset));
						break;
					}
				}
			}
			// 構築したデータを動的配列に追加
			m_meshInverse.push_back(meshInv);
		}
		++meshIt;
	}
}