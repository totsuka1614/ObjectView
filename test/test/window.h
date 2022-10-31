//=============================================================================
//
// ウィンドウ クラス定義 [Window.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef WINDOW_H
#define WINDOW_H

#include "main.h"

class Window
{
public:
	Window() {};
	~Window() {};
	void Release(void);

	HRESULT Create(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
	static Window* GetWindow(){ return m_pWindow; }
	HWND GetWindowHandle() { return m_hWnd; }

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HRESULT ReSizeWindow(HWND hWnd, LPCREATESTRUCT lpcs);
private:

	static Window*  m_pWindow;
	HWND			m_hWnd;					// メイン ウィンドウ ハンドル
	HINSTANCE		m_hInst;				// インスタンス ハンドル
	
};

#endif // !WINDOW_H
