/******************************************************************************
* 
* @file      SceneTitle.cpp
* @brief     タイトルシーン
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

//セレクトボタン列挙
enum SELECT
{
	GAME_SELECT,
	EDIT_SELECT,
	EXIT_SELECT,

	MAX_SELECT
};

/******************************************************************************
* グローバル変数
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
* @note       初期化
* @attention  
******************************************************************************/
void CTitle::Init()
{
	//オブジェクト生成＆グローバル変数へ格納

	//背景
	if (Create<CPolygon>("BackGround"))
		g_bg = GetComponent<CPolygon>("BackGround");

	//タイトルロゴ
	if (Create<CPolygon>("Title"))
		g_title = GetComponent<CPolygon>("Title");

	//ゲームセレクトボタン
	if (Create<UI>("SelectGame"))
		g_select[GAME_SELECT] = GetComponent<UI>("SelectGame");

	//エディタセレクトボタン
	if (Create<UI>("SelectEdit"))
		g_select[EDIT_SELECT] = GetComponent<UI>("SelectEdit");

	//マウス追従パーティクル
	if (Create<CParticle>("Particle"))
		g_particle = GetComponent<CParticle>("Particle");
	
	//マウス追従トレイル
	if (Create<CTrail>("Trail"))
		g_trail = GetComponent<CTrail>("Trail");
	
	//タイトルロゴのばね
	if (Create<CSpring>("Spring"))
		g_spring = GetComponent<CSpring>("Spring");

	//背景初期化
	g_bg->Init();
	g_bg->SetTexture("data/Texture/Title.jpg");
	g_bg->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	//タイトルロゴ初期化
	g_title->Init();
	g_title->SetTexture("data/Texture/Title000.png");
	g_title->SetSize(845.0f, 271.0f);
	g_title->SetPos(-50.0f, 175.0f);

	//セレクトボタン初期化
	for (int i = 0; i < MAX_SELECT; i++)
	{
		if (g_select[i])
			g_select[i]->Init();
	}

	//ゲームセレクトボタン設定
	g_select[GAME_SELECT]->SetTexture("data/Texture/Select003.png");
	g_select[GAME_SELECT]->SetSize(845.0f / 1.5f, 271.0f / 1.5f);
	g_select[GAME_SELECT]->SetPos(0.0f, -100.0f);
	//エディタセレクトボタン設定
	g_select[EDIT_SELECT]->SetTexture("data/Texture/Select004.png");
	g_select[EDIT_SELECT]->SetSize(845.0f / 1.5f, 271.0f / 1.5f);
	g_select[EDIT_SELECT]->SetPos(0.0f, -250.0f);

	//マウス追従パーティクル初期化
	g_particle->Init();
	//マウス追従トレイル初期化
	g_trail->Init();
	//タイトルロゴのばね初期化
	g_spring->Init();

	//BGM再生
	CSound::Play(BGM_TITLE);
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       終了処理
* @attention  
******************************************************************************/
void CTitle::Uninit()
{
	//終了処理
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
* @note       更新処理
* @attention  
******************************************************************************/
void CTitle::Update()
{
	//セレクトボタン更新
	for (int i = 0; i < MAX_SELECT; i++)
	{
		if (g_select[i])
		{
			g_select[i]->Update();

			if (g_select[i]->GetActive())	//カーソルが重ねっているとき、
			{
				if (CInput::GetMouseTrigger(MOUSEBUTTON_L)) //左クリック
				{
					switch (i)
					{
					case GAME_SELECT: //ゲームシーンはまだない
						break;
					case EDIT_SELECT:
						SCENE->Change(SCENE_DEBUG);
						break;
					}
				}
			}
		}
	}

	//更新
	g_particle->Update();
	g_trail->Update();
	g_spring->Update();

	//[デバッグ]０キーでデバッグシーンへ
	if (CInput::GetKeyTrigger(VK_0))
		SCENE->Change(SCENE_DEBUG);

}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       描画
* @attention  
******************************************************************************/
void CTitle::Draw()
{
	//バッファ取得
	CBackBuffer* buffer = BACKBUFFER;

	//Zバッファを無効
	buffer->SetZBuffer(false);
	
	//背景描画
	g_bg->Draw();
	//タイトルロゴ描画
	g_title->Draw();
	//セレクトボタン描画
	for (int i = 0; i < MAX_SELECT; i++)
	{
		if (g_select[i])
			g_select[i]->Draw();
	}
	//マウス追従パーティクル描画
	g_particle->Draw();
	//マウス追従トレイル描画
	g_trail->Draw();
	//タイトルロゴのばね描画
	g_spring->Draw();

	//Zバッファを有効
	buffer->SetZBuffer(true);
}
