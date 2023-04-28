/******************************************************************************
* 
* @file      SceneTitle.cpp
* @brief     �^�C�g���V�[��
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "sceneTitle.h"
#include "GUI.h"
#include "UI.h"
#include "Input.h"
#include "SceneManager.h"
#include "particle.h"
#include "Trail.h"
#include "Spring.h"
#include "Sound.h"

//�Z���N�g�{�^����
enum SELECT
{
	GAME_SELECT,
	EDIT_SELECT,
	EXIT_SELECT,

	MAX_SELECT
};

/******************************************************************************
* �O���[�o���ϐ�
/******************************************************************************/
static CPolygon* g_bg;
static CPolygon* g_title;
static CParticle* g_particle;
static UI* g_select[MAX_SELECT];
static CTrail* g_trail;
static CSpring* g_spring;

/******************************************************************************
* 
* @brief      Init
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ������
* @attention  
******************************************************************************/
void CTitle::Init()
{
	//�I�u�W�F�N�g�������O���[�o���ϐ��֊i�[

	//�w�i
	if (Create<CPolygon>("BackGround"))
		g_bg = GetComponent<CPolygon>("BackGround");

	//�^�C�g�����S
	if (Create<CPolygon>("Title"))
		g_title = GetComponent<CPolygon>("Title");

	//�Q�[���Z���N�g�{�^��
	if (Create<UI>("SelectGame"))
		g_select[GAME_SELECT] = GetComponent<UI>("SelectGame");

	//�G�f�B�^�Z���N�g�{�^��
	if (Create<UI>("SelectEdit"))
		g_select[EDIT_SELECT] = GetComponent<UI>("SelectEdit");

	//�}�E�X�Ǐ]�p�[�e�B�N��
	if (Create<CParticle>("Particle"))
		g_particle = GetComponent<CParticle>("Particle");
	
	//�}�E�X�Ǐ]�g���C��
	if (Create<CTrail>("Trail"))
		g_trail = GetComponent<CTrail>("Trail");
	
	//�^�C�g�����S�̂΂�
	if (Create<CSpring>("Spring"))
		g_spring = GetComponent<CSpring>("Spring");

	//�w�i������
	g_bg->Init();
	g_bg->SetTexture("data/Texture/Title.jpg");
	g_bg->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	//�^�C�g�����S������
	g_title->Init();
	g_title->SetTexture("data/Texture/Title000.png");
	g_title->SetSize(845.0f, 271.0f);
	g_title->SetPos(-50.0f, 175.0f);

	//�Z���N�g�{�^��������
	for (int i = 0; i < MAX_SELECT; i++)
	{
		if (g_select[i])
			g_select[i]->Init();
	}

	//�Q�[���Z���N�g�{�^���ݒ�
	g_select[GAME_SELECT]->SetTexture("data/Texture/Select003.png");
	g_select[GAME_SELECT]->SetSize(845.0f / 1.5f, 271.0f / 1.5f);
	g_select[GAME_SELECT]->SetPos(0.0f, -100.0f);
	//�G�f�B�^�Z���N�g�{�^���ݒ�
	g_select[EDIT_SELECT]->SetTexture("data/Texture/Select004.png");
	g_select[EDIT_SELECT]->SetSize(845.0f / 1.5f, 271.0f / 1.5f);
	g_select[EDIT_SELECT]->SetPos(0.0f, -250.0f);

	//�}�E�X�Ǐ]�p�[�e�B�N��������
	g_particle->Init();
	//�}�E�X�Ǐ]�g���C��������
	g_trail->Init();
	//�^�C�g�����S�̂΂ˏ�����
	g_spring->Init();

	//BGM�Đ�
	CSound::Play(BGM_TITLE);
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �I������
* @attention  
******************************************************************************/
void CTitle::Uninit()
{
	//�I������
	g_bg->Fin();
	g_title->Fin();
	for (int i = 0; i < MAX_SELECT; i++)
	{
		if(g_select[i])
			g_select[i]->Fin();
	}
}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �X�V����
* @attention  
******************************************************************************/
void CTitle::Update()
{
	//�Z���N�g�{�^���X�V
	for (int i = 0; i < MAX_SELECT; i++)
	{
		if (g_select[i])
		{
			g_select[i]->Update();

			if (g_select[i]->GetActive())	//�J�[�\�����d�˂��Ă���Ƃ��A
			{
				if (CInput::GetMouseTrigger(MOUSEBUTTON_L)) //���N���b�N
				{
					switch (i)
					{
					case GAME_SELECT: //�Q�[���V�[���͂܂��Ȃ�
						break;
					case EDIT_SELECT:
						SCENE->Change(SCENE_DEBUG);
						break;
					}
				}
			}
		}
	}

	//�X�V
	g_particle->Update();
	g_trail->Update();
	g_spring->Update();

	//[�f�o�b�O]�O�L�[�Ńf�o�b�O�V�[����
	if (CInput::GetKeyTrigger(VK_0))
		SCENE->Change(SCENE_DEBUG);

}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �`��
* @attention  
******************************************************************************/
void CTitle::Draw()
{
	//�o�b�t�@�擾
	CBackBuffer* buffer = BACKBUFFER;

	//Z�o�b�t�@�𖳌�
	buffer->SetZBuffer(false);
	
	//�w�i�`��
	g_bg->Draw();
	//�^�C�g�����S�`��
	g_title->Draw();
	//�Z���N�g�{�^���`��
	for (int i = 0; i < MAX_SELECT; i++)
	{
		if (g_select[i])
			g_select[i]->Draw();
	}
	//�}�E�X�Ǐ]�p�[�e�B�N���`��
	g_particle->Draw();
	//�}�E�X�Ǐ]�g���C���`��
	g_trail->Draw();
	//�^�C�g�����S�̂΂˕`��
	g_spring->Draw();

	//Z�o�b�t�@��L��
	buffer->SetZBuffer(true);
}
