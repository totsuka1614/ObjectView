//=============================================================================
//
// ウィンドウ クラス [Window.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
//インクルード部
#include "window.h"
#include "imgui_impl_win32.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		_T("AppClass")		// ウインドウのクラス名
#define WINDOW_NAME		_T("シェーダ")		// ウインドウのキャプション名

Window g_window;

Window* Window::m_pWindow = &g_window;

void Window::Release()
{
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, m_hInst);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg) {
	case WM_CREATE:					//ウィンドウが生成された
		return ReSizeWindow(hWnd, (LPCREATESTRUCT)lParam);
	case WM_DESTROY:				//ウィンドウ破棄指示がきた
		PostQuitMessage(0);				//システムにスレッドの終了を要求
		break;
	case WM_KEYDOWN:				//キーボードが押された
		switch (wParam) {
		case VK_ESCAPE:					// ESCキーが押された
			PostMessage(hWnd, WM_CLOSE, 0, 0);	// xが押されたように振舞う
			return 0;
		}
		break;
	case WM_MENUCHAR:
		return MNC_CLOSE << 16;			// Alt+Enter時のBEEPを抑止
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}

HRESULT Window::Create(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex = {
	sizeof(WNDCLASSEX),
	CS_CLASSDC,
	WindowProc,
	0,
	0,
	hInstance,
	LoadIcon(hInstance, MAKEINTRESOURCE(NULL)),
	LoadCursor(nullptr, IDC_ARROW),
	(HBRUSH)(COLOR_WINDOW + 1),
	nullptr,
	CLASS_NAME,
	nullptr
	};

	// COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
		MessageBox(NULL, _T("COMの初期化に失敗しました。"), _T("error"), MB_OK);
		return -1;
	}

	// インスタンス ハンドル保存
	m_hInst = hInstance;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域サイズからウィンドウ サイズ算出		
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION
		| WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX;
	DWORD dwExStyle = 0;
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);

	// ウィンドウの作成
	m_hWnd = CreateWindowEx(dwExStyle,
		CLASS_NAME,
		WINDOW_NAME,
		dwStyle,
		CW_USEDEFAULT,		// ウィンドウの左座標
		CW_USEDEFAULT,		// ウィンドウの上座標
		rc.right - rc.left,	// ウィンドウ横幅
		rc.bottom - rc.top,	// ウィンドウ縦幅
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	// ウインドウの表示
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	return 0;
}

HRESULT Window::ReSizeWindow(HWND hWnd, LPCREATESTRUCT lpcs)
{
	// クライアント領域サイズをSCREEN_WIDTH×SCREEN_HEIGHTに再設定.
	RECT rcClnt;
	GetClientRect(hWnd, &rcClnt);
	rcClnt.right -= rcClnt.left;
	rcClnt.bottom -= rcClnt.top;
	if (rcClnt.right != SCREEN_WIDTH || rcClnt.bottom != SCREEN_HEIGHT) {
		RECT rcWnd;
		GetWindowRect(hWnd, &rcWnd);
		SIZE sizeWnd;
		sizeWnd.cx = (rcWnd.right - rcWnd.left) - rcClnt.right + SCREEN_WIDTH;
		sizeWnd.cy = (rcWnd.bottom - rcWnd.top) - rcClnt.bottom + SCREEN_HEIGHT;
		SetWindowPos(hWnd, nullptr, 0, 0, sizeWnd.cx, sizeWnd.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}

	// IME無効化**かな入力をさせないように
	ImmAssociateContext(hWnd, nullptr);

	return 0;	// -1を返すとCreateWindow[Ex]が失敗する.
}