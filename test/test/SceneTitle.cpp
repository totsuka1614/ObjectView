//=============================================================================
//
// シーンタイトル クラス [SceneTitle.cpp]
// Author : Totsuka Kensuke
//
//=============================================================================
#include "sceneTitle.h"
#include "GUI.h"
#include "UI.h"
#include "Input.h"
#include "SceneManager.h"
#include "TitlePlayer.h"
#include "particle.h"
#include "Trail.h"
#include "Spring.h"
#include "Sound.h"

enum SELECT
{
	GAME_SELECT,
	EDIT_SELECT,
	EXIT_SELECT,

	MAX_SELECT
};

static CPolygon* g_bg;
static CPolygon* g_title;
static Particle* g_particle;
static UI* g_select[MAX_SELECT];
static CTrail* g_trail;
static TitlePlayer* g_player;
static CSpring* g_spring;

void CTitle::Init()
{
	if (Create<CPolygon>("BackGround"))
		g_bg = GetComponent<CPolygon>("BackGround");
	
	if (Create<CPolygon>("Title"))
		g_title = GetComponent<CPolygon>("Title");
	
	if (Create<UI>("SelectGame"))
		g_select[GAME_SELECT] = GetComponent<UI>("SelectGame");

	if (Create<UI>("SelectEdit"))
		g_select[EDIT_SELECT] = GetComponent<UI>("SelectEdit");

	if (Create<TitlePlayer>("TPlayer"))
		g_player = GetComponent<TitlePlayer>("TPlayer");
	
	if (Create<Particle>("Particle"))
		g_particle = GetComponent<Particle>("Particle");
	
	if (Create<CTrail>("Trail"))
		g_trail = GetComponent<CTrail>("Trail");
	
	if (Create<CSpring>("Spring"))
		g_spring = GetComponent<CSpring>("Spring");

	g_bg->Init();
	g_bg->SetTexture("data/Texture/Title.jpg");
	g_bg->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	g_title->Init();
	g_title->SetTexture("data/Texture/Title000.png");
	g_title->SetSize(845.0f, 271.0f);
	g_title->SetPos(-50.0f, 175.0f);

	for (int i = 0; i < MAX_SELECT; i++)
	{
		if (g_select[i])
			g_select[i]->Init();
	}

	g_select[GAME_SELECT]->SetTexture("data/Texture/Select003.png");
	g_select[GAME_SELECT]->SetSize(845.0f / 1.5f, 271.0f / 1.5f);
	g_select[GAME_SELECT]->SetPos(0.0f, -100.0f);

	g_select[EDIT_SELECT]->SetTexture("data/Texture/Select004.png");
	g_select[EDIT_SELECT]->SetSize(845.0f / 1.5f, 271.0f / 1.5f);
	g_select[EDIT_SELECT]->SetPos(0.0f, -250.0f);

	g_player->Init();
	g_particle->Init();
	g_trail->Init();
	g_spring->Init();

	CSound::Play(BGM_TITLE);
}
void CTitle::Uninit()
{
	g_bg->Fin();
	g_title->Fin();
	for (int i = 0; i < MAX_SELECT; i++)
	{
		if(g_select[i])
			g_select[i]->Fin();
	}
}
void CTitle::Update()
{
	
	for (int i = 0; i < MAX_SELECT; i++)
	{
		if (g_select[i])
		{
			g_select[i]->Update();

			if (g_select[i]->GetActive())
			{
				if (CInput::GetMouseTrigger(MOUSEBUTTON_L))
				{
					switch (i)
					{
					case GAME_SELECT:
						break;
					case EDIT_SELECT:
						SCENE->Change(SCENE_DEBUG);
						break;
					}
				}
			}
		}
	}

	g_player->Update();
	g_particle->Update();
	g_trail->Update();
	g_spring->Update();

	if (CInput::GetKeyTrigger(VK_0))
		SCENE->Change(SCENE_DEBUG);

}
void CTitle::Draw()
{
	BackBuffer* buffer = BACKBUFFER;

	buffer->SetZBuffer(false);
	g_bg->Draw();
	g_title->Draw();
	for (int i = 0; i < MAX_SELECT; i++)
	{
		if (g_select[i])
			g_select[i]->Draw();
	}

	g_particle->Draw();
	g_trail->Draw();
	g_spring->Draw();
	buffer->SetZBuffer(true);
	g_player->Draw();

}
