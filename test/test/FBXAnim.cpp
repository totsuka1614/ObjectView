#include "FBX.h"

static FbxManager *g_pManager = NULL;

//-----------------------------------------------------
//
/// @brief データクリア
//
//-----------------------------------------------------
void FBXFile::CleanupAnimation(AnimationInfo &anime)
{
	AnimationInfo::List::iterator it = anime.curveList.begin();
	while (it != anime.curveList.end())
	{
		for (int i = 0; i < AnimationInfo::CurveInfo::CHANNEL_MAX; ++i)
		{
			it->keyList[i].clear();
		}
		++it;
	}
}

//-----------------------------------------------------
//
/// @brief データ取得
//
//-----------------------------------------------------
void FBXFile::GetAnimation(FbxScene *pScene, AnimationInfo &anime, bool isMirror)
{
	CleanupAnimation(anime);
	int animeStackCount = pScene->GetSrcObjectCount<FbxAnimStack>();
	if (animeStackCount == 0)
	{
		return;
	}	
	FbxAnimStack* pStack = pScene->GetSrcObject<FbxAnimStack>(0);
	int animeLayerCount = pStack->GetMemberCount<FbxAnimLayer>();
	if (animeLayerCount == 0)
	{
		return;
	}
	FbxTime time;
	time.SetTime(0, 0, 0, 1, 0, 0, FbxTime::eFrames60);
	anime.totalFrame = static_cast<long>((pStack->GetLocalTimeSpan().GetDuration().Get() + (time.Get() >> 1)) / time.Get());
	CalcAnimation(pStack->GetMember<FbxAnimLayer>(0), pScene->GetRootNode(), anime, isMirror);
}

//-----------------------------------------------------
//
/// @brief ノード中のアニメーション情報取り出し
//
//-----------------------------------------------------
void FBXFile::CalcAnimation(FbxAnimLayer* pLayer, FbxNode* pNode, AnimationInfo &anime, bool isMirror)
{
	// 各要素のキーを抽出
	FbxAnimCurve *pCurves[] =
	{
		pNode->LclTranslation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_X),
		pNode->LclTranslation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Y),
		pNode->LclTranslation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Z),
		pNode->LclRotation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_X),
		pNode->LclRotation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Y),
		pNode->LclRotation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Z),
		pNode->LclScaling.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_X),
		pNode->LclScaling.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Y),
		pNode->LclScaling.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Z),
	};
	AnimationInfo::CurveInfo info;
	for (int i = 0; i < AnimationInfo::CurveInfo::CHANNEL_MAX; ++i)
	{
		GetKeyInfos(pCurves[i], info.keyList[i]);
	}

	if (isMirror)
	{
		MirrorKeys(info.keyList[AnimationInfo::CurveInfo::TRANSLATION_X]);
		MirrorKeys(info.keyList[AnimationInfo::CurveInfo::ROTATION_Y]);
		MirrorKeys(info.keyList[AnimationInfo::CurveInfo::ROTATION_Z]);
	}
	for (int i = 0; i < AnimationInfo::CurveInfo::CHANNEL_MAX; ++i)
	{
		if (info.keyList[i].size() > 0)
		{
			info.name = pNode->GetInitialName();
			anime.curveList.push_back(info);
			break;
		}
	}

	// 子要素検索
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		CalcAnimation(pLayer, pNode->GetChild(i), anime, isMirror);
	}
}

//-----------------------------------------------------
//
/// @brief キー情報取得
//
//-----------------------------------------------------
void FBXFile::GetKeyInfos(FbxAnimCurve *pCurve, AnimationInfo::CurveInfo::List &list)
{
	if (pCurve != NULL)
	{
		FbxTime time;
		time.SetTime(0, 0, 0, 1, 0, 0, FbxTime::eFrames60);

		list.resize(pCurve->KeyGetCount());
		AnimationInfo::CurveInfo::List::iterator it = list.begin();
		while (it != list.end())
		{
			int index = static_cast<int>(it - list.begin());
			it->frame = static_cast<int>((pCurve->KeyGetTime(index).Get() + (time.Get() >> 1)) / time.Get()); // 0.5足して切り上げ
			it->value = pCurve->KeyGetValue(index);
			++it;
		}
	}
}

//-----------------------------------------------------
//
/// @brief キー値のミラーリング
//
//-----------------------------------------------------
void FBXFile::MirrorKeys(AnimationInfo::CurveInfo::List& list)
{
	AnimationInfo::CurveInfo::List::iterator it = list.begin();
	while (it != list.end())
	{
		it->value *= -1.0f;
		++it;
	}
}

// アニメーションの再生指定
void FBXFile::PlayAnime(int animeNo, bool isLoop)
{
	// 再生チェック
	if (animeNo < 0 || animeNo >= MaxAnime || !m_pAnimes[animeNo])
	{
		return;
	}

	m_playAnimeNo = animeNo;
	m_pAnimes[m_playAnimeNo]->playFrame = 0.0f;
	m_pAnimes[m_playAnimeNo]->loop = isLoop;
}
// 遷移ｱﾆﾒｰｼｮﾝの指定
void FBXFile::PlayBlendAnime(int blendNo, float time, bool isLoop)
{
	// 再生チェック
	if (m_playAnimeNo == blendNo)
	{
		return;
	}
	if (blendNo < 0 || blendNo >= MaxAnime || !m_pAnimes[blendNo])
	{
		return;
	}

	m_blendAnimeNo = blendNo;
	m_blendFrame = time * 60;
	m_pAnimes[m_blendAnimeNo]->playFrame = 0.0f;
	m_pAnimes[m_blendAnimeNo]->loop = isLoop;
}

void FBXFile::LoadAnim(const char* file_name)
{
	// 配列に空きがあるか
	int animeIndex = -1;
	for (int i = 0; i < MaxAnime; ++i)
	{
		if (m_pAnimes[i] == nullptr)
		{
			animeIndex = i;
			break;
		}
	}
	if (animeIndex == -1)
	{
		return;
	}

	// アニメーションの読み込み
	m_pAnimes[animeIndex] = new AnimeInfo;
	m_pAnimes[animeIndex]->pAnime = new AnimationInfo;

	// 読み込み初期化
	g_pManager = FbxManager::Create();

	FbxIOSettings *pIO = FbxIOSettings::Create(g_pManager, IOSROOT);
	g_pManager->SetIOSettings(pIO);

	FbxImporter *pImporter = FbxImporter::Create(g_pManager, "");
	if (!pImporter->Initialize(file_name, -1, g_pManager->GetIOSettings()))
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

	GetAnimation(pScene, *m_pAnimes[animeIndex]->pAnime, false);

	pImporter->Destroy();

	// その他初期化
	m_pAnimes[animeIndex]->playFrame = 0.0f;
	m_pAnimes[animeIndex]->loop = false;
	m_pAnimes[animeIndex]->speed = 1.0f;

}

void FBXFile::Step(float speed)
{
	// アニメーションの更新
	if (m_playAnimeNo != -1)
	{
		// 現在再生中のアニメーションを計算
		CalcAnimeFrame(m_playAnimeNo, speed);
		CalcAnime(m_playAnimeNo, m_pBlendBones[0]);

		// 遷移アニメの更新処理
		if (m_blendAnimeNo != -1)
		{
			//--------------
			// モーションの混ぜ合わせ(ブレンドモーション)
			//--------------
			// ブレンドさせるアニメーションの計算
			CalcAnimeFrame(m_blendAnimeNo, speed);
			CalcAnime(m_blendAnimeNo, m_pBlendBones[1]);
			// アニメーションの遷移時間の比率
			float rate = m_pAnimes[m_blendAnimeNo]->playFrame / m_blendFrame;

			for (int i = 0; i < m_pFBXBone->list.size(); ++i)
			{
				// 各骨の各パラメータを補間
				m_pBones[i].param.translate.x =
					m_pBlendBones[0][i].translate.x * (1.0f - rate) +
					m_pBlendBones[1][i].translate.x * rate;

				m_pBones[i].param.translate.y =
					m_pBlendBones[0][i].translate.y * (1.0f - rate) +
					m_pBlendBones[1][i].translate.y * rate;

				m_pBones[i].param.translate.z =
					m_pBlendBones[0][i].translate.z * (1.0f - rate) +
					m_pBlendBones[1][i].translate.z * rate;

				m_pBones[i].param.rotation.x =
					m_pBlendBones[0][i].rotation.x * (1.0f - rate) +
					m_pBlendBones[1][i].rotation.x * rate;

				m_pBones[i].param.rotation.y =
					m_pBlendBones[0][i].rotation.y * (1.0f - rate) +
					m_pBlendBones[1][i].rotation.y * rate;

				m_pBones[i].param.rotation.z =
					m_pBlendBones[0][i].rotation.z * (1.0f - rate) +
					m_pBlendBones[1][i].rotation.z * rate;

				m_pBones[i].param.scale.x =
					m_pBlendBones[0][i].scale.x * (1.0f - rate) +
					m_pBlendBones[1][i].scale.x * rate;

				m_pBones[i].param.scale.y =
					m_pBlendBones[0][i].scale.y * (1.0f - rate) +
					m_pBlendBones[1][i].scale.y * rate;

				m_pBones[i].param.scale.z =
					m_pBlendBones[0][i].scale.z * (1.0f - rate) +
					m_pBlendBones[1][i].scale.z * rate;

			}
			// ブレンドでは2つのアニメーションを常に再生
			// しているので、遷移が終了したら1つのアニメーション
			// のみの再生に戻す
			if (rate >= 1.0f)
			{
				m_playAnimeNo = m_blendAnimeNo;
				m_blendAnimeNo = -1;
			}

			//--------------
		}
		else
		{
			// 遷移しないので単発データをコピー
			for (int i = 0; i < m_pFBXBone->list.size(); ++i)
			{
				m_pBones[i].param = m_pBlendBones[0][i];
			}
		}
	}

	// 骨の姿勢更新
	CalcBone(m_pFBXBone->root, DirectX::XMMatrixIdentity());
}

// アニメーションの時間経過を計算
void FBXFile::CalcAnimeFrame(int animeNo, float speed)
{
	if (animeNo < 0 || animeNo >= MaxAnime || !m_pAnimes[animeNo])
	{
		return;
	}

	m_pAnimes[animeNo]->playFrame += speed;
	// ループ処理
	if (m_pAnimes[animeNo]->loop)
	{
		while (m_pAnimes[animeNo]->playFrame >=
			m_pAnimes[animeNo]->pAnime->totalFrame)
		{
			m_pAnimes[animeNo]->playFrame -=
				m_pAnimes[animeNo]->pAnime->totalFrame;
		}
	}
}
// アニメーションのフレームに応じて骨のポーズを計算
void FBXFile::CalcAnime(int animeNo, MatrixParam* pBones)
{
	// 再生チェック
	if (animeNo < 0 || animeNo >= MaxAnime || !m_pAnimes[animeNo])
	{
		return;
	}

	//--------------
	// アニメーションで動かした骨の計算（ブレンド込みの計算
	//--------------
	// 引数で指定されたアニメーションの情報の取得
	AnimationInfo* pAnime = m_pAnimes[animeNo]->pAnime;

	// アニメーションデータのうち、骨ごとに細かくデータを取得
	for (int i = 0; i < pAnime->curveList.size(); ++i)
	{
		// (存在する骨をアニメーションさせないとおかしくなる)
		int boneIndex = -1;
		for (int j = 0; j < m_pFBXBone->list.size(); ++j)
		{
			if (pAnime->curveList[i].name == m_pFBXBone->list[j].name)
			{
				boneIndex = j;
				break;
			}
		}
		// 対応するアニメーションなし
		if (boneIndex == -1)
		{
			continue;
		}

		// アニメーションの計算は、指定されたフレーム級の時にどんなポーズをとるか計算する。
		// ポーズには移動、回転、拡縮のデータが含まれるため。
		// xyz3つの成分と合わせ、9つ分のパラメータを計算する。

		// 変数名そのままだと、まとめられないので、float型のアドレスとしてまとめる
		MatrixParam* pBone = &pBones[boneIndex];	// 描きこみ先
		float* value[] =
		{
			&pBone->translate.x,&pBone->translate.y,&pBone->translate.z,
			&pBone->rotation.x,&pBone->rotation.y,&pBone->rotation.z,
			&pBone->scale.x,&pBone->scale.y,&pBone->scale.z,
		};

		// パラメータの計算
		for (int j = 0; j < 9; ++j)
		{
			*value[j] = CalcKey(pAnime->curveList[i].keyList[j], m_pAnimes[animeNo]->playFrame, *value[j]);
		}
	}


	//--------------
}

// アニメーションのフレームに対応した姿勢を計算
float FBXFile::CalcKey(AnimationInfo::CurveInfo::List& keys, float frame, float defValue)
{
	//--------------
	// アニメーションのキー計算
	//--------------

	//拡縮や移動はキーがないこともあるので、例外処理を行う
	if (keys.size() == 0)
	{
		return defValue;
	}
	// キーが一つだけでは補間できないので、そのまま使用
	else if (keys.size() == 1)
	{
		return keys[0].value;
	}
	// 二つ以上で補間
	else
	{
		// 二つのフレームの間を補完する(i)と(i - 1)のフレームを調べるため、初期値は1から始める
		// (0だと配列外にアクセスする)
		for (int i = 1; i < keys.size(); i++)
		{
			// キーの開始時間と終了時間の間に挟まれているか
			float startFrame = keys[i - 1].frame;
			float endFrame = keys[i].frame;
			if (startFrame <= frame && frame < endFrame)
			{
				// キーの値
				float startValue = keys[i - 1].value;
				float endValue = keys[i].value;

				// 比率 = (現在値 - 開始値) / (目標値 - 開始値)
				float rate = (frame - startFrame) / (endFrame - startFrame);

				// 線形補間の式 = (目標 - 開始値) * 倍率 + 開始値
				return	(endValue - startValue) * rate + startValue;	// 補間した結果
			}
		}
		// 一番最後のキーよりも遅いフレームの場合、最後のフレームの値をそのまま使用
		return keys[keys.size() - 1].value;
	}




	//--------------
	return 0.0f;
}