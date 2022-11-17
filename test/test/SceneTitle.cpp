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

static UI* g_bg;
static UI* g_title;
static UI* g_select[MAX_SELECT];

void CTitle::Init()
{
	if (Create<UI>("BackGround"))
		g_bg = GetComponent<UI>("BackGround");
	
	if (Create<UI>("Title"))
		g_title = GetComponent<UI>("Title");
	
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
	g_bg->Update();
	g_title->Update();
	for (int i = 0; i < MAX_SELECT; i++)
	{
		if (g_select[i])
			g_select[i]->Update();
	}

	if (CInput::GetKeyTrigger(VK_0))
		SceneManager::Get()->Change(SCENE_DEBUG);

}
void CTitle::Draw()
{
	BackBuffer* buffer = BackBuffer::GetBuffer();

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
