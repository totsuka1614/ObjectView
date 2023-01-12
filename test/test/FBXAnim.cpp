#include "FBX.h"

static FbxManager *g_pManager = NULL;

//-----------------------------------------------------
//
/// @brief �f�[�^�N���A
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
/// @brief �f�[�^�擾
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
/// @brief �m�[�h���̃A�j���[�V���������o��
//
//-----------------------------------------------------
void FBXFile::CalcAnimation(FbxAnimLayer* pLayer, FbxNode* pNode, AnimationInfo &anime, bool isMirror)
{
	// �e�v�f�̃L�[�𒊏o
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

	// �q�v�f����
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		CalcAnimation(pLayer, pNode->GetChild(i), anime, isMirror);
	}
}

//-----------------------------------------------------
//
/// @brief �L�[���擾
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
			it->frame = static_cast<int>((pCurve->KeyGetTime(index).Get() + (time.Get() >> 1)) / time.Get()); // 0.5�����Đ؂�グ
			it->value = pCurve->KeyGetValue(index);
			++it;
		}
	}
}

//-----------------------------------------------------
//
/// @brief �L�[�l�̃~���[�����O
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

// �A�j���[�V�����̍Đ��w��
void FBXFile::PlayAnime(int animeNo, bool isLoop)
{
	// �Đ��`�F�b�N
	if (animeNo < 0 || animeNo >= MaxAnime || !m_pAnimes[animeNo])
	{
		return;
	}

	m_playAnimeNo = animeNo;
	m_pAnimes[m_playAnimeNo]->playFrame = 0.0f;
	m_pAnimes[m_playAnimeNo]->loop = isLoop;
}
// �J�ڱ�Ұ��݂̎w��
void FBXFile::PlayBlendAnime(int blendNo, float time, bool isLoop)
{
	// �Đ��`�F�b�N
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
	// �z��ɋ󂫂����邩
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

	// �A�j���[�V�����̓ǂݍ���
	m_pAnimes[animeIndex] = new AnimeInfo;
	m_pAnimes[animeIndex]->pAnime = new AnimationInfo;

	// �ǂݍ��ݏ�����
	g_pManager = FbxManager::Create();

	FbxIOSettings *pIO = FbxIOSettings::Create(g_pManager, IOSROOT);
	g_pManager->SetIOSettings(pIO);

	FbxImporter *pImporter = FbxImporter::Create(g_pManager, "");
	if (!pImporter->Initialize(file_name, -1, g_pManager->GetIOSettings()))
	{
		return;
	}

	// �ǂݍ���
	FbxScene *pScene = FbxScene::Create(g_pManager, "originalScene");
	pImporter->Import(pScene);
	// �O�p��
	FbxGeometryConverter geometryConverter(g_pManager);
	geometryConverter.Triangulate(pScene, true);
	geometryConverter.RemoveBadPolygonsFromMeshes(pScene);
	// �}�e���A���ŕ���
	geometryConverter.SplitMeshesPerMaterial(pScene, true);

	GetAnimation(pScene, *m_pAnimes[animeIndex]->pAnime, false);

	pImporter->Destroy();

	// ���̑�������
	m_pAnimes[animeIndex]->playFrame = 0.0f;
	m_pAnimes[animeIndex]->loop = false;
	m_pAnimes[animeIndex]->speed = 1.0f;

}

void FBXFile::Step(float speed)
{
	// �A�j���[�V�����̍X�V
	if (m_playAnimeNo != -1)
	{
		// ���ݍĐ����̃A�j���[�V�������v�Z
		CalcAnimeFrame(m_playAnimeNo, speed);
		CalcAnime(m_playAnimeNo, m_pBlendBones[0]);

		// �J�ڃA�j���̍X�V����
		if (m_blendAnimeNo != -1)
		{
			//--------------
			// ���[�V�����̍������킹(�u�����h���[�V����)
			//--------------
			// �u�����h������A�j���[�V�����̌v�Z
			CalcAnimeFrame(m_blendAnimeNo, speed);
			CalcAnime(m_blendAnimeNo, m_pBlendBones[1]);
			// �A�j���[�V�����̑J�ڎ��Ԃ̔䗦
			float rate = m_pAnimes[m_blendAnimeNo]->playFrame / m_blendFrame;

			for (int i = 0; i < m_pFBXBone->list.size(); ++i)
			{
				// �e���̊e�p�����[�^����
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
			// �u�����h�ł�2�̃A�j���[�V��������ɍĐ�
			// ���Ă���̂ŁA�J�ڂ��I��������1�̃A�j���[�V����
			// �݂̂̍Đ��ɖ߂�
			if (rate >= 1.0f)
			{
				m_playAnimeNo = m_blendAnimeNo;
				m_blendAnimeNo = -1;
			}

			//--------------
		}
		else
		{
			// �J�ڂ��Ȃ��̂ŒP���f�[�^���R�s�[
			for (int i = 0; i < m_pFBXBone->list.size(); ++i)
			{
				m_pBones[i].param = m_pBlendBones[0][i];
			}
		}
	}

	// ���̎p���X�V
	CalcBone(m_pFBXBone->root, DirectX::XMMatrixIdentity());
}

// �A�j���[�V�����̎��Ԍo�߂��v�Z
void FBXFile::CalcAnimeFrame(int animeNo, float speed)
{
	if (animeNo < 0 || animeNo >= MaxAnime || !m_pAnimes[animeNo])
	{
		return;
	}

	m_pAnimes[animeNo]->playFrame += speed;
	// ���[�v����
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
// �A�j���[�V�����̃t���[���ɉ����č��̃|�[�Y���v�Z
void FBXFile::CalcAnime(int animeNo, MatrixParam* pBones)
{
	// �Đ��`�F�b�N
	if (animeNo < 0 || animeNo >= MaxAnime || !m_pAnimes[animeNo])
	{
		return;
	}

	//--------------
	// �A�j���[�V�����œ����������̌v�Z�i�u�����h���݂̌v�Z
	//--------------
	// �����Ŏw�肳�ꂽ�A�j���[�V�����̏��̎擾
	AnimationInfo* pAnime = m_pAnimes[animeNo]->pAnime;

	// �A�j���[�V�����f�[�^�̂����A�����Ƃɍׂ����f�[�^���擾
	for (int i = 0; i < pAnime->curveList.size(); ++i)
	{
		// (���݂��鍜���A�j���[�V���������Ȃ��Ƃ��������Ȃ�)
		int boneIndex = -1;
		for (int j = 0; j < m_pFBXBone->list.size(); ++j)
		{
			if (pAnime->curveList[i].name == m_pFBXBone->list[j].name)
			{
				boneIndex = j;
				break;
			}
		}
		// �Ή�����A�j���[�V�����Ȃ�
		if (boneIndex == -1)
		{
			continue;
		}

		// �A�j���[�V�����̌v�Z�́A�w�肳�ꂽ�t���[�����̎��ɂǂ�ȃ|�[�Y���Ƃ邩�v�Z����B
		// �|�[�Y�ɂ͈ړ��A��]�A�g�k�̃f�[�^���܂܂�邽�߁B
		// xyz3�̐����ƍ��킹�A9���̃p�����[�^���v�Z����B

		// �ϐ������̂܂܂��ƁA�܂Ƃ߂��Ȃ��̂ŁAfloat�^�̃A�h���X�Ƃ��Ă܂Ƃ߂�
		MatrixParam* pBone = &pBones[boneIndex];	// �`�����ݐ�
		float* value[] =
		{
			&pBone->translate.x,&pBone->translate.y,&pBone->translate.z,
			&pBone->rotation.x,&pBone->rotation.y,&pBone->rotation.z,
			&pBone->scale.x,&pBone->scale.y,&pBone->scale.z,
		};

		// �p�����[�^�̌v�Z
		for (int j = 0; j < 9; ++j)
		{
			*value[j] = CalcKey(pAnime->curveList[i].keyList[j], m_pAnimes[animeNo]->playFrame, *value[j]);
		}
	}


	//--------------
}

// �A�j���[�V�����̃t���[���ɑΉ������p�����v�Z
float FBXFile::CalcKey(AnimationInfo::CurveInfo::List& keys, float frame, float defValue)
{
	//--------------
	// �A�j���[�V�����̃L�[�v�Z
	//--------------

	//�g�k��ړ��̓L�[���Ȃ����Ƃ�����̂ŁA��O�������s��
	if (keys.size() == 0)
	{
		return defValue;
	}
	// �L�[��������ł͕�Ԃł��Ȃ��̂ŁA���̂܂܎g�p
	else if (keys.size() == 1)
	{
		return keys[0].value;
	}
	// ��ȏ�ŕ��
	else
	{
		// ��̃t���[���̊Ԃ�⊮����(i)��(i - 1)�̃t���[���𒲂ׂ邽�߁A�����l��1����n�߂�
		// (0���Ɣz��O�ɃA�N�Z�X����)
		for (int i = 1; i < keys.size(); i++)
		{
			// �L�[�̊J�n���ԂƏI�����Ԃ̊Ԃɋ��܂�Ă��邩
			float startFrame = keys[i - 1].frame;
			float endFrame = keys[i].frame;
			if (startFrame <= frame && frame < endFrame)
			{
				// �L�[�̒l
				float startValue = keys[i - 1].value;
				float endValue = keys[i].value;

				// �䗦 = (���ݒl - �J�n�l) / (�ڕW�l - �J�n�l)
				float rate = (frame - startFrame) / (endFrame - startFrame);

				// ���`��Ԃ̎� = (�ڕW - �J�n�l) * �{�� + �J�n�l
				return	(endValue - startValue) * rate + startValue;	// ��Ԃ�������
			}
		}
		// ��ԍŌ�̃L�[�����x���t���[���̏ꍇ�A�Ō�̃t���[���̒l�����̂܂܎g�p
		return keys[keys.size() - 1].value;
	}




	//--------------
	return 0.0f;
}