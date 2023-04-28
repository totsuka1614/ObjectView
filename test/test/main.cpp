/******************************************************************************
* 
* @file      main.cpp
* @brief     メイン処理
* @author    Totsuka Kensuke
* @date      2023/04/18
* @note      
* @attention 
* 
******************************************************************************/

//インクルード部
#include "main.h"
#include "backbuffer.h"
#include "window.h"
#include "GUI.h"
#include "Camera.h"
#include "Input.h"
#include "Sound.h"
#include "SceneManager.h"
#include "light.h"

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

/******************************************************************************
* 
* @brief      _tWinMain
* @param[in]  hInstance
* @param[in]  hPrevInstance
* @param[in]  lpCmdLine
* @param[in]  nCmdShow
* @return     int 
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       ウィンドウメイン関数
* @attention  
* 
******************************************************************************/
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 未使用宣言
	UNREFERENCED_PARAMETER(lpCmdLine);		// 未使用宣言

	CWindow* window = CWindow::GetWindow();

	if(window->Create(hInstance, hPrevInstance,lpCmdLine,nCmdShow))
		return -1;
	
	MSG msg;

	// DirectXの初期化
	if (FAILED(Init(window->GetWindowHandle(), true))) {
		return -1;
	}

	GUI::Get()->Init();
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

/******************************************************************************
*       
* @brief      FPS処理関係は外部ソースを利用
* 
******************************************************************************/
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
	if (SleepTime > 0 && SleepTime < 18)//大きく変動がなければSleepTimeは1〜17の間に納まる
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

/******************************************************************************
* 
* @brief      Init
* @param[in]  hWnd
* @param[in]  bWindow
* @return     HRESULT
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       初期化
* @attention  
* 
******************************************************************************/
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;
	
	//バッファ取得
	hr = BACKBUFFER->Init();
	if (FAILED(hr))
		return hr;

	// サウンド初期化
	CSound::Init();

	//カメラ初期化
	CCamera::Get()->Init();
	
	//シーンマネージャー初期化
	SCENE->Init();

	return hr;
}

/******************************************************************************
* 
* @brief      Uninit
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       終了処理
* @attention  
* 
******************************************************************************/
void Uninit(void)
{
	//終了処理
	GUI::Get()->Release();
	SCENE->Uninit();
	CInput::Fin();
	CSound::Fin();
	BACKBUFFER->Release();

}

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       更新処理
* @attention  
* 
******************************************************************************/
void Update(void)
{
	//更新処理
	CInput::Update();
	GUI::Get()->Update();
	CCamera::Get()->Update();
	SCENE->Update();
	GUI::Get()->Display();

	// サウンド更新
	CSound::Update();	// 必ずUpdate関数の末尾で実行.
}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/04/18
* @note       描画処理
* @attention  
* 
******************************************************************************/
void Draw(void)
{
	//画面クリア
	CCamera::Get()->Clear();

	//描画処理
	SCENE->Draw();
	GUI::Get()->Draw();

	//描画完了処理
	BACKBUFFER->FinishRendering();
}