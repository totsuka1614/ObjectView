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

enum SELECT
{
	GAME_SELECT,
	EDIT_SELECT,
	EXIT_SELECT,

	MAX_SELECT
};

static CPolygon* g_bg;
static CPolygon* g_title;
static UI* g_select[MAX_SELECT];

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

	g_bg->Init();
	g_bg->SetTexture("data/Texture/Title.jpg");
	g_bg->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	g_title->Init();
	g_title->SetTexture("data/Texture/TitleName.png");
	g_title->SetSize(845.0f/1.5f, 271.0f/1.5f);
	g_title->SetPos(0.0f, 150.0f);

	for (int i = 0; i < MAX_SELECT; i++)
	{
		if (g_select[i])
			g_select[i]->Init();
	}

	g_select[GAME_SELECT]->SetTexture("data/Texture/Select002.png");
	g_select[GAME_SELECT]->SetSize(845.0f / 3.0f, 271.0f / 3.0f);
	g_select[GAME_SELECT]->SetPos(0.0f, -100.0f);

	g_select[EDIT_SELECT]->SetTexture("data/Texture/Select001.png");
	g_select[EDIT_SELECT]->SetSize(845.0f / 3.0f, 271.0f / 3.0f);
	g_select[EDIT_SELECT]->SetPos(0.0f, -200.0f);


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
	buffer->SetZBuffer(true);
}
