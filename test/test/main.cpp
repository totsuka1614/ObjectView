//=============================================================================
//
// メイン処理 [main.cpp]
// Author : TotsukaKensuke
//
//=============================================================================

//インクルード部
#include "main.h"
#include "backbuffer.h"
#include "model.h"
#include "window.h"
#include "GUI.h"
#include "FBX.h"
#include "Camera.h"
#include "Input.h"
#include "grid.h"
//ライブラリのリンク
#pragma comment(lib, "winmm")
#pragma comment(lib, "imm32")
#pragma comment(lib, "d3d11")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT Init(HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
int							g_nCountFPS;			// FPSカウンタ
Model *model;
Grid *grid;

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 未使用宣言
	UNREFERENCED_PARAMETER(lpCmdLine);		// 未使用宣言

	Window* window = Window::GetWindow();

	if(window->Create(hInstance, hPrevInstance,lpCmdLine,nCmdShow))
		return -1;
	
	MSG msg;

	// DirectXの初期化
	if (FAILED(Init(window->GetWindowHandle(), true))) {
		return -1;
	}

	GUI::Get()->Init();
	grid = new Grid;
	grid->Init();
	CInput::Init();
	Fps fps;

	fps.InitFps();

	// メッセージループ
	for (;;) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else {
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {

			fps.CalculationFps();

			// 更新処理
			Update();
			// 描画処理
			Draw();
			fps.CalculationSleep();
		}
	}

	// タイマ設定を元に戻す
	timeEndPeriod(1);

	// 終了処理
	Uninit();

	window->Release();

	// COM終了処理
	CoUninitialize();

	return (int)msg.wParam;
}

#pragma region FPS処理関係
void Fps::InitFps()
{
	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&StartTime);//現在の時間を取得（1フレーム目）
}
/**
 * @fn
 * FPS計算処理
 * @brief FPSを計算してiCountに収納
 */
void Fps::CalculationFps()
{
	//FPSの計算
	if (iCount == 60)//カウントが60の時の処理
	{
		QueryPerformanceCounter(&NowTime);//現在の時間を取得（60フレーム目）
		//FPS = 1秒 / 1フレームの描画にかかる時間
		g_dFps = 1000.0 / (static_cast<double>((NowTime.QuadPart - StartTime.QuadPart) * 1000 / Freq.QuadPart) / 60.0);
		iCount = 0;//カウントを初期値に戻す
		StartTime = NowTime;//1フレーム目の時間を現在の時間にする
	}
	iCount++;//カウント+1
}
/**
 * @fn
 * FPS待機処理
 * @brief FPSが60を超えないよう、スリープする
 */
void Fps::CalculationSleep()
{
	//Sleepさせる時間の計算
	QueryPerformanceCounter(&NowTime);//現在の時間を取得
	//Sleepさせる時間ms = 1フレーム目から現在のフレームまでの描画にかかるべき時間ms - 1フレーム目から現在のフレームまで実際にかかった時間ms
	//                  = (1000ms / 60)*フレーム数 - (現在の時間ms - 1フレーム目の時間ms)
	SleepTime = static_cast<DWORD>((1000.0 / 60.0) * iCount - (NowTime.QuadPart - StartTime.QuadPart) * 1000 / Freq.QuadPart);
	if (SleepTime > 0 && SleepTime < 18)//大きく変動がなければSleepTimeは1～17の間に納まる
	{
		timeBeginPeriod(1);
		Sleep(SleepTime);
		timeEndPeriod(1);
	}
	else//大きく変動があった場合
	{
		timeBeginPeriod(1);
		Sleep(1);
		timeEndPeriod(1);
	}
}
/**
 * @fn
 * FPSゲッター
 * @brief FPS値を返す処理
 * @return g_dFPS
 */
double Fps::GetFps()
{
	return g_dFps;
}
#pragma endregion

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;
	
	hr = BackBuffer::GetBuffer()->Init();
	if (FAILED(hr))
		return hr;

	model = new Model;
	model->Init();

	// ラスタライズ設定
	//D3D11_RASTERIZER_DESC rd;
	//ZeroMemory(&rd, sizeof(rd));
	//rd.FillMode = D3D11_FILL_SOLID;
	//rd.CullMode = D3D11_CULL_NONE;	// カリング無し(両面描画)
	//g_pDevice->CreateRasterizerState(&rd, &g_pRs[0]);
	//rd.CullMode = D3D11_CULL_FRONT;	// 前面カリング(裏面描画)
	//g_pDevice->CreateRasterizerState(&rd, &g_pRs[1]);
	//rd.CullMode = D3D11_CULL_BACK;	// 背面カリング(表面描画)
	//g_pDevice->CreateRasterizerState(&rd, &g_pRs[2]);
	//g_pDeviceContext->RSSetState(g_pRs[2]);

	

	//// 深度ステンシルステート生成
	//CD3D11_DEFAULT def;
	//CD3D11_DEPTH_STENCIL_DESC dsd(def);
	//dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	//g_pDevice->CreateDepthStencilState(&dsd, &g_pDSS[0]);
	//CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	//dsd2.DepthEnable = FALSE;
	//g_pDevice->CreateDepthStencilState(&dsd2, &g_pDSS[1]);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	GUI::Get()->Release();
	delete model;
	delete grid;
	CInput::Fin();
	BackBuffer::GetBuffer()->Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	CInput::Update();
	GUI::Get()->Update();
	CCamera::Get()->Update();
	model->Update();

	GUI::Get()->Display();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	BackBuffer::GetBuffer()->StartRendering();

	GUI::Get()->Draw();
	grid->Draw();
	model->Draw();
	BackBuffer::GetBuffer()->FinishRendering();
}